#include "kernel/types.h"
#include "user/user.h"

#define READ 0
#define WRITE 1

// TIPS : have to ensure every pipe that do not need is closed 

void prime(int* left)
{
	close(left[WRITE]);
	int pri,tmp,right[2];
	if(read(left[READ],&pri,sizeof(int))==0)
	{
		close(left[READ]);
		exit(0);
	}
	
	printf("prime %d\n",pri);
	pipe(right);
	int pid=fork();
	if(pid<0)
	{
		fprintf(2,"fork error!");
		exit(1);
	}
	else if(pid>0){
		close(right[READ]);
		while(read(left[READ],&tmp,sizeof(int))!=0)
		{
			if(tmp%pri==0) continue;
			write(right[WRITE],&tmp,sizeof(int));
		}
		close(right[WRITE]);
		wait(0);
		exit(0);
	}
	else
	{
		prime(right);
		wait(0);
		exit(0);
	}
}
int main(int argc, char const *argv[])
{
	int pid,fd[2];
	pipe(fd);

    if((pid=fork())<0)
	{
		fprintf(2,"fork error!");
		exit(1);
	}
	else if(pid>0)
	{
		close(fd[READ]);
		for(int i=2;i<=35;i++)
		{
			write(fd[WRITE],&i,sizeof(int));
		}
		close(fd[WRITE]);
		wait(0);
		exit(0);
	}
	else{//child
		prime(fd);
		exit(0);
	}
}