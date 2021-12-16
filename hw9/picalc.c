/*   
 *   Project: "PI calculation using random area coverage"
 *   calculation of the number hits inside circle of area=1
 *   Parallel with OpenMP, distribution of work
 *
 *   created     author     comments
 *   04/12/2021  I.Zacharov initial release
 */
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#include "piprog.h"

#ifndef _OPENMP
   #define omp_get_max_threads() 1
#endif

#define CACHE_LINE_SIZE		64      // cache line 64 Bytes

typedef struct timethread {
	double elapsed;
	char dummy[CACHE_LINE_SIZE-sizeof(double)];
} timethread_t;

void ompinit(void)
{
#pragma omp parallel
    { }
}

long long numcirc(                    // returns hits inside circle
			   inpar_t * input)       // input data, eg. total number of trials, seed 
{
  long long num_trials = input->npoints;
  long long genseed    = input->seed;
  int mpi_th = input->mpith;
  int mpi_np = input->mpinp;
  unsigned long long ncirc;
  long rseed = genseed;
  int    maxomp = omp_get_max_threads();
  timethread_t ttimes[maxomp];
  double tstart = omp_get_wtime();

#pragma omp parallel default(none) firstprivate(num_trials,genseed,mpi_np) shared(ncirc,ttimes)
  {
    double x, y, t, dres1, dres2;
    struct drand48_data rbuf;
    int    nomp = omp_get_num_threads();
    int    iomp = omp_get_thread_num();
    long rseed = (1LL + mpi_np*iomp) * genseed; //unique seed from all ids
    unsigned long long local_ncirc = 0;
    unsigned long long i, ilast;
    double tstart = omp_get_wtime();

    srand48_r(rseed, &rbuf);

    #pragma omp for schedule(static) reduction(+:ncirc)
    for( i = 0; i < num_trials; i++ ) { // split work statically

	    drand48_r(&rbuf, &dres1);      // re-entrant random num gen
	    drand48_r(&rbuf, &dres2);      // [0..1)

	    x = 2.0 * dres1 - 1;	       // place the circle around 0	
	    y = 2.0 * dres2 - 1;
	    t = x*x + y*y;

	    if( t <= 1.0 ) ncirc++;
        ilast = i;
    }
    double tend = omp_get_wtime();
    ttimes[iomp].elapsed = tend - tstart;
    //printf("omp: %d %d seed: %lld iter: %lld\n",iomp, nomp, rseed,ilast);
  } // end omp parallel region

  double tend = omp_get_wtime();
  double tlapse = tend - tstart;

  if ( input->pflag ) {
	int i;
	fprintf(stderr,"\tmpi_thread: %d of %d  points: %lld  seed: %lld ncirc: %lld T: %.2f",
					mpi_th, mpi_np, num_trials,rseed,ncirc, tlapse);
        fprintf(stderr," [");
        for(i=0; i < maxomp; i++) fprintf(stderr," %.2f", ttimes[i].elapsed);
        fprintf(stderr," ]\n");
  }

  return ncirc;
}

