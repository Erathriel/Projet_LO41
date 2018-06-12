#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define KEY 123456789

void main(int argc, char *argv[])
{
	pid_t pid;
	int shmid;
	key_t key;
	if(argc) {
		shmid = shmget(KEY, sizeof(argv[0]), 0666);
		argv[0]=(char*)shmat(shmid,NULL,0);
	}
	else if(argc==0) {
		shmid=shmget(KEY, sizeof(argv[0]), 0666);
		
	}	
	else exit(0);
}
