/*   
 *   include file for the project 
 *   "PI calculation using random area coverage"
 *   contains common data definitions
 *
 *   created     author     comments
 *   04/12/2021  I.Zacharov initial release
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>    // clock_gettime

#define NPOINTS		800000000	// number trials: good for 10 sec running on 1 cpu
//#define NPOINTS		8000000000	// number trials
#define GENSEED		1235791		// prime random seed

#define NANOSEC_in_SEC 1000000000LL
#define dsecond(tx,ty) ((double)((ty.tv_sec)-(tx.tv_sec))+(double)((ty.tv_nsec)-(tx.tv_nsec))/NANOSEC_in_SEC)

typedef struct inpar {
	long long npoints;
	long long seed;
    int       mpinp;
    int       mpith;
        int pflag;
} inpar_t;

long long numcirc(inpar_t *);

void getparams(int , char *[], inpar_t *);

void initomp(void);
