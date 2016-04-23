#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
int main(){
	pid_t pid;
	int filedes[2];
	char buf[200];
	int childPid;
	pipe(filedes);
	pid=fork();
	if(pid==0){
		printf("I am a child process.I will send a message to my father.\n");
		char c[]="This is a message from child process,its pid is";
		childPid=getpid();
		char s[200]={0};
		sprintf(s,"%s%d",c,childPid);
		write(filedes[1],s,sizeof(s));
		close(filedes[0]);
		close(filedes[1]);
	}else{
		printf("I am a father process.I am waiting my child to send a message.\n");
		waitpid(pid,0,0);
		read(filedes[0],buf,sizeof(buf));
		printf("%s\n",buf);
		close(filedes[0]);
		close(filedes[1]); 
	}
	return 0;
	
} 
