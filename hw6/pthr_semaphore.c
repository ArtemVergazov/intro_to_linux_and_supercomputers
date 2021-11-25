/* Created by Artem.Vergazov@skoltech.ru
 *
 * Syntax of run: export N_ITER=N
 * 		  ./pthr_semaphore
 * 		  export -n N_ITER
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define N_ITER_init 500000000

static long long sum = 0;
int N_ITER = N_ITER_init;
sem_t sem;

void *func(void *arg) {
    long n = *(long *)arg;
    long i;
    for (i = 0; i < N_ITER; ++i) {
        sem_wait(&sem);
	sum += n;
	sem_post(&sem);
    }
    pthread_exit(NULL);
}

int main(int argc, char **argv) {
    char *cn_it = getenv("N_ITER"); // get an environment variable
    if (cn_it) if ((N_ITER = atoi(cn_it)) == 0) { perror("atoi"); exit(1); }
    if (N_ITER < 0 || N_ITER >= N_ITER_init) N_ITER = N_ITER_init;
    long n1, n2;
    int p1, p2, p3, p4;

    pthread_t tid1, tid2; // thread id
    pthread_attr_t attr;

    pthread_attr_init(&attr);
    sem_init(&sem, 0, 1);

    n1 = 1;
    if (pthread_create(&tid1, &attr, func, &n1) !=0) { perror("pthread_create"); fprintf(stderr, "err no. %d\n", p1); }
    n2 = -1;
    if ((p2 = pthread_create(&tid2, &attr, func, &n2)) != 0) { perror("pthread_create"); fprintf(stderr, "err no. %d\n", p2); }
    if ((p3 = pthread_join(tid1, NULL)) != 0) { perror("pthread_create"); fprintf(stderr, "err no. %d\n", p3); }
    if ((p4 = pthread_join(tid2, NULL)) != 0) { perror("pthread_create"); fprintf(stderr, "err no. %d\n", p4); }

    printf("N_ITER = %d, \nsum = %lld\n", N_ITER, sum);
	
    return 0;
}





























