#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <errno.h>

#define NTHR 8

sigset_t mask;
pthread_t tid[NTHR];
int block[NTHR];
int alarmthread;

void delay(int nsec)
{
    struct timespec ts;
    
    ts.tv_sec = nsec;
    ts.tv_nsec = 0;
    nanosleep(&ts, NULL);
}

void handler(int signo)
{
    int i;
    pthread_t id;
    
    id = pthread_self();
    for (i = 0; i < NTHR; i++)
        if (tid[i] == id)
            break;
    if (i >= NTHR)
        printf(" Thread Principale: Attrape le  signal %d\n", signo);
    else
        printf("thread %d: Attrape le signal %d\n", i+1, signo);
}

void *thr_fn(void *arg)
{
    int id;
    
    id = (int)arg;
    printf("thread %d commence...\n", id);
    if (block[id-1] != 0) {
        printf("thread %d bloque le SIGALRM...\n", id);
        if (pthread_sigmask(SIG_BLOCK, &mask, NULL))
            fprintf(stderr,"thread %d: ne peut pas bloquer le SIGALRM", id);
    }
    if (alarmthread == id) {
        printf("thread %d: met en place l'alarme...\n", id);
        alarm(5);
    }
    pause();
    printf("thread %d: exiting\n", id);
    exit(id);
}

int main(int argc, char *argv[])
{
    struct sigaction act;
    int i, err, id;
    int doblock = 0;
    
    setbuf(stdout, NULL);
    sigemptyset(&mask);
    sigaddset(&mask, SIGALRM);
    act.sa_handler = handler;
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);
    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "main") == 0) {
            doblock = 1;
        } else if (argv[i][0] == 'a') {
            alarmthread = atoi(&argv[i][1]);
            if (alarmthread > NTHR || alarmthread < 1)
               fprintf(stderr,"thread ID must be between 1 and %d\n");
        } else {
            id = atoi(argv[i]);
            if (id > NTHR || id < 1)
                fprintf(stderr,"thread ID must be between 1 and %d\n");
            block[id-1] = 1;
        }
    }
    if (sigaction(SIGALRM, &act, NULL) < 0)
        fprintf(stderr,"ne peut pas intaller le signal pour le handler");
    for (i = 0; i < NTHR; i++) {
        err = pthread_create(&tid[i], NULL, thr_fn, (void *)(i+1));
        if (err != 0)
            fprintf(stderr,"Ne peut pas creer le thread");
    }
    if (doblock != 0) {
        printf(" le thread principal bloque le  SIGALRM...\n");
        if (pthread_sigmask(SIG_BLOCK, &mask, NULL))
            fprintf(stderr,"le thread principal: ne peut pas bloquer SIGALRM");
    }
    if (alarmthread == 0) {
        printf("le Thread principal met en place l'alarme...\n");
        alarm(5);
    }
    delay(10);
    printf("thread principale retourne apres expiration du delai\n");
    exit(0);
}
