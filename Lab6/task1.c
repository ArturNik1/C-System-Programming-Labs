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
#include <sys/stat.h>
#include <fcntl.h>


void execute(cmdLine *pCmdLine){
    if(pCmdLine->inputRedirect != NULL){
        close(0);
        dup(open(pCmdLine->inputRedirect, O_RDONLY));
    }                           
    if(pCmdLine->outputRedirect != NULL){
        close(1);
        dup(open(pCmdLine->outputRedirect,O_WRONLY));
    }
    
    if(execvp(pCmdLine->arguments[0],pCmdLine->arguments)==-1) {
        perror("Failed");
        if(pCmdLine->inputRedirect != NULL){
            close(0);
        }
        if(pCmdLine->outputRedirect != NULL){
            close(1);
        }
        freeCmdLines(pCmdLine);
        _exit(1);
    }
}

int main(int argc, char *argv[])
{
    while(1) {
        int debug = 0;
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
        
        int pid = fork();
        if (pid == 0) {
            execute(c);
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

