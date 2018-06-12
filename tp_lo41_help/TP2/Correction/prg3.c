#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
int tube[2];
pipe(tube);
switch(fork())
	{
	case 0://fils
		close(tube[1]);
		/*close(0);
		dup(tube[0]);*/
		dup2(tube[0],0);
		execlp("wc","wc","-l",NULL);
		exit(0);
		break;
	default://père
		close(tube[0]);
		/*close(1);
		dup(tube[1]);*/
		dup2(tube[1],1);
		execlp("ls","ls","-l",NULL);
		wait(NULL);
	break;
	}
}
