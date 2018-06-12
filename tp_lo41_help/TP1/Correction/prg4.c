#include <stdio.h>
#include <unistd.h>		/* pour pid_t getppid(void) */
#include <stdlib.h>
#include <signal.h>
#include <errno.h>

#include <sys/wait.h>


int main(void)
{
    pid_t pid;
    siginfo_t info;
    pid = fork();

    if (pid == -1) {
	fprintf(stderr, "fork() impossible, errno = %d \n", errno);
	exit(EXIT_FAILURE);
    }

    if (pid == 0) {

	while (1) {

	    fprintf(stdout, "FILS : PID= %ld , PPID = %ld \n",
		    (long) getpid(), (long) getppid());
	    sleep(3);

	    fprintf(stdout, "FILS %d va stopper \n", getpid());
	    printf("Envoi du signal SIGSTOP \n");
	    raise(SIGSTOP);
	}
    }


    while (1) {

	fprintf(stderr, " [Pere] : en attente \n");
	if (waitid(P_ALL, 0, &info, WSTOPPED) == 0) {
	    if (info.si_code == CLD_STOPPED) {
		printf("[PERE] : mon fils %ld arrete par signal %d \n",
		       (long) pid, info.si_status);
		sleep(2);
		printf("[PERE] je le relance \n");
		kill(info.si_pid, SIGCONT);
	    }
	}
    }
    return EXIT_SUCCESS;
}
