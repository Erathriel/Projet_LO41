#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char* argv[])
{
int pid;
    switch(pid=fork())
    {
	case 0:
	execl("./fils", "fils", "abc", NULL);
break;
default:
	wait(NULL);
	printf("Je suis le papa\n");
   }
}
