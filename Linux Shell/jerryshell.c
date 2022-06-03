#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

void tokenize(char* string,char* tokv[]) //인자로 토큰화
{
	int i = 1;
	tokv[0] = strtok(string," ");
	while(1) {
		tokv[i] = strtok(NULL," ");
		if(tokv[i] == NULL) return;
		i++;
		}
}
int back_check(char* m_argv[]) //&여부를 판단해 bool로 바꾸고 바뀐 argv를 만듦
{
	int i;
	for(i = 0; m_argv[i] != NULL; i++)
	{
		if(strcmp(m_argv[i],"&") == 0) {
			m_argv[i] = NULL;
			return 1;	
		}
	}
	return 0;
}

void redirect(char* m_argv[])
{
	int i = 0;
	int check = 0;	//redirection check
	int fd;
	for(i; m_argv[i] != NULL; i++)
	{
		if(strcmp(m_argv[i],">") == 0)
		{
			check = 1;
			break;
		}
	}
	if(check == 0) return;	//do not redirect
	
	fd = open(m_argv[i+1],O_RDWR | O_CREAT, 0641);
	if(!fd) {	//open failure
		printf("cannot open file\n"); 
		return;
	}
	if(dup2(fd,1) < 0) {	//duplicate failure
		printf("dup fail\n");
		return;
	}
	m_argv[i] = 0;
	m_argv[i+1] = 0;
}

int main() {
	char str[500];
	char* m_argv[1000];
	int backg = 0;	//background processing flag
	int pid_fork;

	printf("welcome to my shell <:3)~ I'm jerry\n");

	while(1)
	{
		fflush(stdout);
		fflush(stdin);

		printf(">> ");
		fgets(str,sizeof(str),stdin);
		if(strcmp(str,"exit\n") == 0) return 0;
		str[strlen(str)-1] = 0;

		tokenize(str,m_argv);	//tokenize
		
		backg = back_check(m_argv);	//background processing flag set

		if((pid_fork = fork()) < 0 ) {	//fork failure
			printf("fork failure\n");
			exit(1);
			}
		if(pid_fork == 0)	//child process
		{
			if(backg == 1) sleep(1); //background processing
			
			redirect(m_argv);	//redirection
			
			if(execvp(m_argv[0],m_argv))
			{ 
				fprintf(stderr,"program execution error : %s\n", strerror(errno));
				return 1; 
			}
			exit(1);
		}
		else //parent process
		{
			if(backg != 1) wait();

		}
		//parent exit
	}
}
