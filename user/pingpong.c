#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char const *argv[])
{
	int pid,fdp2c[2],fdc2p[2];
	char p2c[16],c2p[16];
	pipe(fdp2c);
	pipe(fdc2p);

	if((pid=fork())<0)
	{
		fprintf(2,"error!");
		exit(1);
	}
	else if(pid==0)//child
	{
		close(fdp2c[1]);
		close(fdc2p[0]);

		read(fdp2c[0],p2c,sizeof(p2c));
		close(fdp2c[0]);
		printf("%d: received %s\n",getpid(),p2c);

		write(fdc2p[1],"pong",4);
		close(fdc2p[1]);
		exit(0);
	}
	else{
		close(fdp2c[0]);
		close(fdc2p[1]);

		write(fdp2c[1],"ping",4);
		close(fdp2c[1]);

		read(fdc2p[0],c2p,sizeof(c2p));
		close(fdc2p[0]);
		printf("%d: received %s\n",getpid(),c2p);

		exit(0);
	}
}