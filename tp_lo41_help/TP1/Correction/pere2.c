#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char* argv[])
{
int pid, pid2;
pid=fork();
if(pid==0)
	execlp("ls", "l", NULL);
pid2=fork();
if(pid2==0)
	execlp("ps", "l", NULL);
waitid
printf("Fin du proc pere\n");
}

