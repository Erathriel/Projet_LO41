#include <pthread.h>
 
/* fonction executee par  chaque thread */ 
void *fonc_thread(void *k) {

	/*printf("Thread numero %d : mon tid est %d\n", (int) k,pthread_self());
	for (;;)
	compteur[(int) k]++;*/
	if(k

} 

int main(void) {
	int i, num;
	pthread_t pth_id[2];

/* creation des threads */ 
	for (num = 0; num < 2; num++) {
		pthread_create(pth_id + num, 0, fonc_thread, (void *) num);
		printf("Main: thread numero %d creee: id = %d\n", 
			num,pth_id[num]);
	}
	exit(0);
}
