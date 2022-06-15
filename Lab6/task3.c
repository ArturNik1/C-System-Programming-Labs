#include <stdio.h>
#include <limits.h>
#include "LineParser.h"
#include <string.h>
#include <signal.h>
#include <linux/limits.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int ** createPipes(int nPipes){
    int** pipes;
    pipes=(int**) calloc(nPipes, sizeof(int));
    int i;
    for (i=0; i<nPipes;i++){
        pipes[i]=(int*) calloc(2, sizeof(int));
        pipe(pipes[i]);
    }  
    return pipes;

    }
void releasePipes(int **pipes, int nPipes){
    int i;
    for (i=0; i<nPipes;i++){
        free(pipes[i]);
    
    }  
    free(pipes);
}
int *leftPipee(int **pipes, cmdLine *pCmdLine){
    if (pCmdLine->idx == 0) return NULL;
    return pipes[pCmdLine->idx -1];
}

int *rightPipee(int **pipes, cmdLine *pCmdLine){
    if (pCmdLine->next == NULL) return NULL;
    return pipes[pCmdLine->idx];
}

void executeMultiple(cmdLine* pCmdLine, int count) {
    int** pipes = createPipes(count);
    int pids[count];
    int i = 0;
    
    while (pCmdLine != NULL) {
        int id = fork();
        pids[i] = id;
        i++;
        int* leftPipe = leftPipee(pipes, pCmdLine);
        int* rightPipe = rightPipee(pipes, pCmdLine);
        
        if (id == 0) {
            if (leftPipe != NULL) {
                close(0);
                dup(leftPipe[0]);
                close(leftPipe[0]);
            }
            if (rightPipe != NULL) {
                close(1);
                dup(rightPipe[1]);
                close(rightPipe[1]);
            }
            execvp(pCmdLine->arguments[0], pCmdLine->arguments);
            break;
        }
        else {
            if (rightPipe != NULL) close(rightPipe[1]);
            if (leftPipe != NULL) close(leftPipe[0]);
            if (pCmdLine->next == NULL) {
                int j;
                for (j = 0; j <= count; j++) waitpid(pids[j], NULL, 0);
                if (pipes) releasePipes(pipes, count);
            }
        }
        pCmdLine = pCmdLine->next;
    }
}

void execute(cmdLine *pCmdLine){
    int fd[2];
    pipe(fd);
    int id = fork();
    if (id == 0) {
        close(1);
        close(fd[0]);
        dup(fd[1]);
        close(fd[1]);    
        if(execvp(pCmdLine->arguments[0], pCmdLine->arguments) == -1) {
            perror("Failed");
            _exit(1);
        }
    }
    else {
        close(fd[1]);
        int id2 = fork();
        if (id2 == 0) {
            close(0);
            close(fd[1]);
            dup(fd[0]);
            close(fd[0]);
            pCmdLine = pCmdLine->next;
            if(execvp(pCmdLine->arguments[0], pCmdLine->arguments) == -1) {
                perror("Failed");
                _exit(1);
            }
        }
        else {
            close(fd[0]);
            waitpid(id, NULL, 0);
            waitpid(id2, NULL, 0);
        }
    }
    
    if(execvp(pCmdLine->arguments[0],pCmdLine->arguments)==-1) {
        perror("Failed");
        _exit(1);
    }
}

int main(int argc, char *argv[])
{
    while(1) {
        int debug = 0;
        int count = 0;
        if (argc > 1 && strncmp("-d", argv[1], 2) == 0) debug = 1;
        
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
        }
        
        cmdLine* cur = c;
        while (cur != NULL) {
            count++;
            cur = cur->next;
        }
        
        int pid = fork();
        if (pid == 0) {
            if (count < 1) execute(c);
            else executeMultiple(c, count);
            exit(1);
        }
        
        if (debug == 1) {
            fprintf(stderr,"pid: %d\n",pid);
            fprintf(stderr,"command: %s\n",input);
        }
        
        if(c -> blocking) waitpid(pid,NULL,0);

        freeCmdLines(c);

    }
    return 0;
}

