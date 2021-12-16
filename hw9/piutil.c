/*   
 *   Project "PI calculation using random area coverage"
 *   utility programs
 *
 *   created     author     comments
 *   04/12/2021  I.Zacharov initial release
 */
#include <stdio.h>
#include <stdlib.h>

#include "piprog.h"

#define OPTFLAGS	"s:t:a"

const char optflags[] = OPTFLAGS ;   //option flags

void usage(int argc, char *argv[])
{
        fprintf(stderr,"usage: %s [-%c <long>] [-%c <long>] [-%c] \n",argv[0],optflags[0],optflags[2],optflags[4]);
        exit(0);
}
void getparams(int argc, char *argv[], inpar_t *input)
{
        const char *flags = optflags;
        char *param = NULL;
        int opt;
        int flag = 0;
        extern char * optarg;
        long long par1 = GENSEED;
        long long par2 = NPOINTS;
        long long npfactor = 1;
        int pflag = 0;

    while ( (opt = getopt(argc, argv, flags)) != -1 ) {
                if ( opt == optflags[0] ) {    
                     param = optarg;
                     par1 = atoll(param);
		}
		else if ( opt == optflags[2] ) {   
                     param = optarg;
                     npfactor = atoll(param);
		}
		else if ( opt == optflags[4] ) {
		     pflag = 1;
		}
		else {
		     usage(argc, argv);
		     break;
		}	
	}
	input->seed = par1;
	input->npoints = par2 * npfactor;
    input->pflag = pflag;
    //printf("par2 %ld npfact %ld\n",par2, npfactor);
}

