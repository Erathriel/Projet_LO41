#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <time.h>


#define SIZE_TAB 5
#define KEY 123456789

int main(int argc, char *argv[])
{
  int somme=0;
  struct data { int t[SIZE_TAB];};
  int shmId;
  int tab[SIZE_TAB];
  key_t key = ftok("./",'h'); // Key creation
  shmId = shmget(key,SIZE_TAB,IPC_CREAT | 0644); // Segment creation
  struct data* p=(struct data *)shmat(shmId,NULL,0); // Attache segement 
  pid_t pid_fils = fork(); // Create son
  srand(time(NULL));
  if (pid_fils == -1) {
    perror("fork"); // error
  }
  else if (pid_fils == 0) {
    struct data* f = shmat(shmId,NULL,0); // Attache segment 
    for (int i = 0; i < SIZE_TAB; ++i)
    {
      tab[i]=rand();
      printf("Num : %d\n", tab[i]);
    }
    sprintf(f->t, tab); // Son write a message in the segment
    if(shmdt(f) == -1){ // Segment detache
      printf("Error segment can't be detached \n");
    }
    else
    {
      printf("Segment detached \n");
    }
  }
  else {
    sleep(1); // Wait the death of his son
    for (int i = 0; i < SIZE_TAB; ++i)
    {
      somme+=p->t[i];
      printf("%d\n", p->t[i]);
    }
    shmdt(p);
    shmctl(shmId, IPC_RMID, NULL); // Delete segment
  }
  return 0;
}