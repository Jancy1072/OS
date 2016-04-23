#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>

int main(int argc,char *argv[],char *envp[])
{
    pid_t pid;
    pid=fork();
    if(pid==0) //child process
    {
        printf("\nI'm a child process!\n");     
        char *newargv[] = {"ls","-1",NULL};
        execve("/bin/ls",newargv,envp);
		printf("I can't print\n");
        exit(0);
    }else{  
        waitpid(pid,0,0);
        printf("I should print finally\n");
    }
    return 0;
}


