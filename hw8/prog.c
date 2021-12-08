/* Created by Artem.Vergazov@skoltech.ru on 12.7.21. 
 *
 * Compile with: module load mpi/openmpi-3.1.2
 *               mpicc -o pim -O3 prog.c -lm
 *
 * Syntax of run: mpirun -np 4 -mca btl_base_warn_component_unused 0 ./pim
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h> // sqrt
#include <mpi.h>
#include <time.h>

#define NPOINTS 8000000000 // number trials
#define GENSEED	1235791	// prime random seed
#define OPTFLAGS "st:"

#define NANOSEC_in_SEC 1000000000LL
#define dsecond(tx, ty) ((double)((ty.tv_sec) - (tx.tv_sec)) + (double)((ty.tv_nsec) - (tx.tv_nsec)) / NANOSEC_in_SEC)
#define _POSIX_C_SOURCE 200809L

extern int optind;
const char optflags[] = OPTFLAGS;

void usage(int argc, char *argv[]) {
    fprintf(stderr, "usage: %s [-%c <long>] [-%c <ull>] \n", argv[0], optflags[0], optflags[1]);
    exit(0);
}

int main(int argc, char *argv[]) {
    long ncirc = 0;
    double pi, dpi;
    int numthrd;
    int  mythrid;

    unsigned long long num_trials = NPOINTS;
    const long gen_seed = GENSEED;		
	
    if (argc == 2) {
    	printf("using custom number of trials\n");
	num_trials = strtoll(argv[1], NULL, 10);
	printf("num_trials = %d\n", num_trials);
	// const long gen_seed = strtol(argv[2], NULL, 10);		
    }
	
    int rank, size;
    struct timespec tstart, tend;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numthrd);
    MPI_Comm_rank(MPI_COMM_WORLD, &mythrid);

    clock_gettime(CLOCK_REALTIME, &tstart);

// Instead of explicitly specifying a block to parallelize in openmp, we write a program that will be run independently by a few processes in MPI.
// #pragma omp parallel default(none) firstprivate(num_trials) shared(ncirc)
    {
	double x, y, t, dres1, dres2;
	struct drand48_data rbuf;
	long rseed = (mythrid + 1) * gen_seed;
	unsigned long long i;
	unsigned long long local_ncirc = 0;

	srand48_r(rseed, &rbuf);

	for (i = 0; i < num_trials; ++i) { // do not split the work
            drand48_r(&rbuf, &dres1); // re-entrant random num gen
	    drand48_r(&rbuf, &dres2); // [0..1)

            x = 2.0 * dres1 - 1; // place the circle around 0
	    y = 2.0 * dres2 - 1;
            t = x * x + y * y;

            if (t <= 1.0) ++local_ncirc;
	}
// #pragma omp atomic
	// ncirc += local_ncirc;

        // We can get the results of all processes in one variable with MPI_Reduce instead of doing `ncirc += local_ncirc`.
	MPI_Reduce(&local_ncirc, &ncirc, 1, MPI_LONG_LONG_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    } // end parallel region
	
	clock_gettime(CLOCK_REALTIME, &tend);
	double tlaps = dsecond(tstart, tend);
	
	num_trials *= numthrd; // all threads same work
	pi = 4.0 * (double)ncirc / (double)num_trials;
	dpi = pi * sqrt(2.0 / num_trials);
	if (mythrid == 0) {
	    // printf("argc = %d\n", argc);
            // fprintf(stdout, "The argv parameters are \n");
	    // int i;
	    // for (i = 0; i < argc; ++i) {
            //    printf(" %d ", strtol(argv[i], NULL, 10));
	    // }

	    fprintf(stdout, "\nTrials: %ld Ncirc: %ld Threads: %d Elapsed: %.2f PI: %.8lf dpi: %.1g\n", num_trials, ncirc, numthrd, tlaps, pi, dpi);
	}
	MPI_Finalize();
	return 0;
}

