#include<stdio.h>
#include<unistd.h>

int main(int argc,char * argv[])
{
    pid_t pid1;
    pid_t pid2;
    pid1 = fork();
	if(0 == pid1){  //child process1
        printf("\nThis is child process1,pid is %d\n",getpid());
    }else{
          pid2=fork();
          if(0 == pid2){  //child process2
               printf("\nThis is child process2,pid is %d\n",getpid());
          }else{
                printf("\nThis is parent process,pid is %d\n",getpid());
          }
    }   
     waitpid(pid1,0,0);
     waitpid(pid2,0,0);
    return 0;
}

