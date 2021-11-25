/* Created by Artem.Vergazov@skoltech.ru on 11.25.21.
 *
 * Syntax of run: export N_ITER=N
 * 		  ./pthr_atomic
 * 		  export -n N_ITER
 *
 * or:            ./pthr_atomic
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdatomic.h>

#define N_ITER_init 500000000
#define N_THRS 16

atomic_long sum = 0;
int N_ITER = N_ITER_init;

void *func(void *arg) {
    long n = *(long *)arg;
    long i;
    for (i = 0; i < N_ITER; ++i) {
	sum += n; // note: only update op is atomic
    }
    pthread_exit(NULL);
}

int main(int argc, char **argv) {
    char *cn_it = getenv("N_ITER"); // get an environment variable
    if (cn_it && (N_ITER = atoi(cn_it)) == 0) { perror("atoi"); exit(1); }
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

    printf("starting %d threads: ", nth);
    for (i = 0; i < nth; ++i) {
    	na[i] = (i & 1) ? -1 : 1;
	printf("na[i] = %ld'n'", na[i]);
	if (pthread_create(&(tid[i]), &attr, func, &(na[i])) != 0) { perror("pthread_create"); }
        printf("	[ %d: %x %d ]", i, tid[i], na[i]);
    }
    printf("\n");
    for (i = 0; i < nth; ++i) {
	if (pthread_join(tid[i], NULL) != 0) { perror("pthread_join"); }
    }
    printf("N_ITER = %d, \nsum = %lld\n", N_ITER, sum);
    return 0;
}





























