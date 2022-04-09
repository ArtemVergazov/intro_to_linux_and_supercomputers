/* Created by Artem.Vergazov@skoltech.ru
 * Revision 1.1 on 12.24.21 @1.13AM.
 *
 * Compile with: gcc -o pthr_semaphore pthr_semaphore.c -pthread
 * Syntax of run: export N_ITER=N
 * 				  export NUM_THREADS=N_T
 * 				  ./pthr_semaphore
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define N_ITER_init	500000000
#define N_THRS 16

static long long sum = 0;
int N_ITER = N_ITER_init;
sem_t sem;

void *func(void *arg) {
	long n = *(long *)arg;
	long i;
	for (i = 0; i < N_ITER; ++i) {
		sem_wait(&sem); // semaphore
		sum += n;
		sem_post(&sem); // semaphore
	}
	pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
	char *cn_it = getenv("N_ITER"); // get an environment variable
	if (cn_it) if ((N_ITER = atoi(cn_it)) == 0) { perror("atoi"); exit(1); }
	if (N_ITER < 0 || N_ITER >= N_ITER_init) N_ITER = N_ITER_init;
	long na[N_THRS];
	pthread_t tid[N_THRS];
	pthread_attr_t attr;
	int nth = 1;
	char *cnth = getenv("NUM_THREADS"); // get an environment variable
	int i;

	if (cnth) if ((nth = atoi(cnth)) == 0) { perror("atoi"); exit(1); }
	if (nth < 0 || nth >= N_THRS) nth = 2;
	pthread_attr_init(&attr);

	sem_init(&sem, 0, 1); // semaphore initialization
	
	printf("starting %d threads: ", nth);
	for (i = 0; i < nth; ++i) {
		na[i] = (i & 1) ? -1 : 1;
		printf("na[i] = %ld'n'", na[i]);
		if (pthread_create(&(tid[i]), &attr, func, &(na[i])) != 0) perror("pthread_create");
		printf("	[ %d: %x %d ]", i, tid[i], na[i]);
	}
	printf("\n");
	for (i = 0; i < nth; ++i) {
		if (pthread_join(tid[i], NULL) != 0) perror("pthread_join");
	}
	printf("N_ITER = %d, \nsum = %lld\n", N_ITER, sum);
	return 0;
}

