/*   
 *   Project "PI calculation using random area coverage"
 *   Main program
 *
 *   created     author     comments
 *   04/12/2021  I.Zacharov initial release
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>    // clock_gettime
#include <math.h>    // sqrt
#include <mpi.h>
#include <omp.h>

#include "piprog.h"

int main(int argc, char *argv[])
{
    long ncirc = 0;
    double pi, dpi;
    int mpi_np;
    int mpi_id;
    inpar_t input;
    unsigned long long num_trials;
    int rank, size;
    int num_omp = omp_get_max_threads();
    struct timespec tstart, tend;
    char procname[MPI_MAX_PROCESSOR_NAME]; 
    int pnamelen = sizeof(procname);

    MPI_Init(&argc, &argv);
    MPI_Comm_size( MPI_COMM_WORLD, &mpi_np);
    MPI_Comm_rank( MPI_COMM_WORLD, &mpi_id);

    ompinit();
    if ( mpi_id == 0 ) getparams(argc,argv, &input);
    MPI_Bcast(&input, sizeof(inpar_t) , MPI_BYTE, 0, MPI_COMM_WORLD);
    input.mpith = mpi_id;
    input.mpinp = mpi_np;
    input.seed *= (mpi_id + 1);

    MPI_Get_processor_name(procname, &pnamelen);
    if(input.pflag) fprintf(stderr,"id %d n_omp: %d %s parameters: %lld %lld\n",mpi_id, num_omp, procname, input.npoints, input.seed);
    MPI_Barrier(MPI_COMM_WORLD);

    clock_gettime(CLOCK_REALTIME,&tstart);

    const long long ncircL = numcirc(&input);

    MPI_Allreduce(&ncircL, &ncirc, 1, MPI_LONG_LONG_INT, MPI_SUM, MPI_COMM_WORLD);
    //MPI_Reduce(&ncircL, &ncirc, 1, MPI_LONG_LONG_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    clock_gettime(CLOCK_REALTIME,&tend); 
    double tlaps = dsecond(tstart,tend);

    num_trials = input.npoints*mpi_np;             // all threads do same work
    pi = 4.0 * (double) ncirc/ (double) num_trials;
    dpi = pi*sqrt(2.0/num_trials);
    if(mpi_id == 0 || input.pflag)
    fprintf(stderr,"%s Trials: %-12ld Ncirc: %-12ld Seed: %-12ld Threads(MPI,OMP): %d %d Elapsed: %.2f  PI: %.8lf dpi: %.1g\n",
		    procname, num_trials, ncirc, input.seed, mpi_np, num_omp, tlaps, pi, dpi);

    if(mpi_id == 0 )
    fprintf(stdout,"MASTER %s Trials: %-12ld Ncirc: %-12ld Seed: %-12ld Threads(MPI,OMP): %d %d Elapsed: %.2f  PI: %.8lf dpi: %.1g\n",
		    procname, num_trials, ncirc, input.seed, mpi_np, num_omp, tlaps, pi, dpi);

    MPI_Finalize();
    return 0;
}
