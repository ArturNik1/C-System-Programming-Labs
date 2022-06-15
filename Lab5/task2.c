#define _GNU_SOURCE
#include <stdio.h>
#include <limits.h>
#include "LineParser.h"
#include <string.h>
#include <signal.h>
#include <linux/limits.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <wait.h>
#define TERMINATED  -1
#define RUNNING 1
#define SUSPENDED 0

typedef struct process{
    cmdLine* cmd;                         /* the parsed command line*/
    pid_t pid; 		                  /* the process id that is running the command*/
    int status;                           /* status of the process: RUNNING/SUSPENDED/TERMINATED */
    struct process *next;	                  /* next process in chain */
} process;

void execute(cmdLine *pCmdLine){
    if(execvp(pCmdLine->arguments[0],pCmdLine->arguments)==-1) {
        perror("Failed");
        _exit(1);
    }
}

void addProcess(process** process_list, cmdLine* cmd, pid_t pid) {
    process* newProcess = (process*)calloc(1, sizeof(process));
    newProcess -> cmd = cmd;
    newProcess -> pid = pid;
    newProcess -> status = RUNNING;
    newProcess -> next = *process_list;
    *process_list = newProcess;
}

char* statusToString(int status) {
    if (status == TERMINATED) return "TERMINATED";
    else if (status == SUSPENDED) return "SUSPENDED";
    else return "RUNNING";
}

void updateProcessList(process** process_list) {
    process* cur = *process_list;
    int status;
    while (cur != NULL) {
        int result = waitpid(cur->pid, &status, WNOHANG|WUNTRACED|WCONTINUED);
        if(waitpid(cur->pid, &status, WNOHANG|WUNTRACED|WCONTINUED)) {
            if (result == -1 ||WIFEXITED(status)) cur->status = TERMINATED;
            else if (WIFSTOPPED(status)) cur->status = SUSPENDED;
            else if (WIFCONTINUED(status)) cur->status = RUNNING;
        }
        cur = cur->next;
    }
}


void recPrintProcessList(process* cur, process* prev){
	if(cur != NULL){
		cmdLine* cmd = cur->cmd;
		printf("%d", (int)cur -> pid);
		printf("\t %s ",cmd->arguments[0]);
		printf("\t \t %s \n",statusToString(cur -> status));
		if(cur->status == TERMINATED){
		    if(prev != NULL){
		        prev->next = cur->next;
		        freeCmdLines(cur->cmd);
		        free(cur);
		        cur = prev;
		    }
		}
		recPrintProcessList(cur->next, cur);
		
    }
}

void printProcessList(process** process_list){
	updateProcessList(process_list);
    printf("Process ID: \t Command: \t Status: \n");
	recPrintProcessList(*process_list, NULL);
	process *first = *process_list;
	if(first != NULL){
		if(first ->status == TERMINATED){
            process* temp = first;
			*process_list = first -> next;
            freeCmdLines(temp -> cmd);
            free(temp);
        }
    }
}

void freeProcessList(process* process_list) {
    process* n;
    while (process_list != NULL) {
        n = process_list -> next;
        freeCmdLines (process_list-> cmd);
        free (process_list);
        process_list = n;
    }
}


void updateProcessStatus(process* process_list, int pid, int status) {
    process* cur = process_list;
    while (cur != NULL && cur->pid != pid) cur = cur->next;
    if (cur != NULL) cur->status = status;
}



int main(int argc, char *argv[])
{
    process* pr = NULL;
    process** process_list = &pr;
    int debug = 0;
    if (argc > 1 && strncmp("-d", argv[1], 2) == 0) debug = 1;
    
    
    while(1) {
        char path[PATH_MAX];
        getcwd(path, PATH_MAX);
        printf("%s\n", path);
        char input [2048] ;
        fgets(input,2048,stdin);
        
        cmdLine* c = parseCmdLines(input);
        
        if(strcmp(input,"quit\n")==0)
            exit(0);
        else if (strcmp(c->arguments[0],"cd") == 0) {
            char newDir[PATH_MAX];
            strcpy(newDir,c->arguments[1]);
            if(chdir(newDir)!=0 || c->argCount < 2)
                fprintf(stderr,"%s\n","cd failed...");
            continue;
        }
        else if (strcmp(c->arguments[0], "showprocs") == 0) {
            printProcessList(process_list);
            continue;
        }
        else if (strcmp(c->arguments[0], "nap") == 0) {
            int pid2 = fork();
            if(pid2 == 0){
                kill(atoi(c-> arguments[2]), SIGTSTP);
                sleep(atoi(c-> arguments[1]));
                kill( atoi(c-> arguments[2]), SIGCONT);
                exit(1);
            }
            continue;
        }
        
        else if (strcmp(c-> arguments[0] ,"stop")== 0){
            kill(atoi(c->arguments[1]), SIGINT);
            continue;
        }
        
        int pid = fork();
        if (pid == 0) {
            execute(c);
            exit(1);
        }
        
        if (debug == 1) {
            fprintf(stderr,"pid: %d\n",pid);
            fprintf(stderr,"command: %s\n",input);
        }
        signal(SIGTSTP, SIG_DFL);
        addProcess(process_list,c, pid);
        if(c -> blocking) waitpid(pid,NULL,0);
        
        freeCmdLines(c);

    }
    return 0;
}

