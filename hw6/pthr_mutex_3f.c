/* Created by Artem.Vergazov@skoltech.ru on 11.25.21.
 *
 * Syntax of run: ./pthr_mutex_3f
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N_ITER 500000000

static long long sum = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *func(void *arg) {
    long n = *(long *)arg;
    long i;
    for (i = 0; i < N_ITER; ++i) {
        pthread_mutex_lock(&mutex);
	sum += n;
	pthread_mutex_unlock(&mutex);
    }
    pthread_exit(NULL);
}

int main(int argc, char **argv) {
    long n;
    int p1, p2, p3, p4;
    pthread_t tid1, tid2; // thread id
    pthread_attr_t attr;

    pthread_attr_init(&attr);

    n = 1;
    if (pthread_create(&tid1, &attr, func, &n) != 0) { perror("pthread_create"); fprintf(stderr, "err no. %d\n", p1); }
    n = -1;
    if ((p2 = pthread_create(&tid2, &attr, func, &n)) != 0) { perror("pthread_create"); fprintf(stderr, "err no. %d\n", p2); }
    if ((p3 = pthread_join(tid1, NULL)) != 0) { perror("pthread_create"); fprintf(stderr, "err no. %d\n", p3); }
    if ((p4 = pthread_join(tid2, NULL)) != 0) { perror("pthread_create"); fprintf(stderr, "err no. %d\n", p4); }

    printf("N_ITER = %d, sum = %lld\n", N_ITER, sum);
	
    return 0;
}





























