#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#define NUM_THREADS 3
void thread1_process(void)
{
printf("Raclette");
e
}
int main(void)
{
  pthread_attr_t thread_attr;
	pthread_t th1;

  if (pthread_attr_init (&thread_attr) != 0) {
    fprintf (stderr, "pthread_attr_init error");
    exit (1);
  }

  if (pthread_attr_setdetachstate (&thread_attr, PTHREAD_CREATE_DETACHED) != 0) {
    fprintf (stderr, "pthread_attr_setdetachstate error");

    exit (1);
  }
  if (pthread_create (&th1, &thread_attr, thread1_process, NULL) < 0) {
          fprintf (stderr, "pthread_create error for thread 1\n");
        exit (1);
  }
}


