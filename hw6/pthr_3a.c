/* Created by Artem.Vergazov@skoltech.ru
 *
 * RACE condition in access to shared variable.
 *
 * Syntax of run: ./pthr_3a
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N_ITER 500000000

static long long sum = 0;

void *func(void *arg) {
    long n = *(long *)arg;
    long i;
    for (i = 0; i < N_ITER; ++i) {
	sum += n;
    }
    pthread_exit(NULL);
}

int main(int argc, char **argv) {
    long n1, n2;
    int p1, p2, p3, p4;
    pthread_t tid1, tid2; // thread id
    pthread_attr_t attr;

    pthread_attr_init(&attr);

    n1 = 1;
    if (pthread_create(&tid1, &attr, func, &n1) !=0) { perror("pthread_create"); fprintf(stderr, "err no. %d\n", p1); }
    n2 = -1;
    if ((p2 = pthread_create(&tid2, &attr, func, &n2)) !=0) { perror("pthread_create"); fprintf(stderr, "err no. %d\n", p2); }

    if ((p3 = pthread_join(tid1, NULL)) !=0) { perror("pthread_create"); fprintf(stderr, "err no. %d\n", p3); }
    if ((p4 = pthread_join(tid2, NULL)) !=0) { perror("pthread_create"); fprintf(stderr, "err no. %d\n", p4); }

    printf("sum = %lld\n", sum);
	
    return 0;
}

