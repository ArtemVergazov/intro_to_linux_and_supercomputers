/* Demonstrate reopen of std I/O streams.
 * Exercise 3.3.
 * Program will copy file in first argument to second argument, or std streams.
 * prog -s in out
 * Created by Artem.Vergazov@skoltech.ru 11.16.21 @9:13PM.
 */
 
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>
#include <fcntl.h>

// Definitions.
#define SIZEBUF 128 // size of the input buffer
#define OPTFLAGS "s:" // option flags as compiler symbolic string

// Global variables.
extern int optind;
const char optflags[] = OPTFLAGS;

// Functions.
void usage(int argc, char *argv[]) {
    fprintf(stderr, "Usage: %s [-%c <int>] in out \n", argv[0], optflags[0]);
    exit(0);
}
long getparams(int argc, char *argv[]) {
    const char *flags = optflags;
    char *param = NULL;
    int opt;
    int flag = 0;
    extern char *optarg;
    long parsize = 0;

    while ((opt = getopt(argc, argv, flags)) != -1) {
    	if ( opt == optflags[0] ) { // note: switch needs const in case
            param = optarg;
	    parsize = atoll(param);
	    // break; shoud NOT break the while loop to collect all flags/parameters
	} else usage(argc, argv);
    }

    return parsize;
}


int main(int argc, char *argv[]) {
    int f1 = fileno(stdin); // file descriptor for standard streams
    int f2 = fileno(stdout);
    int readflg = O_RDONLY;
    int writflg = O_CREAT | O_WRONLY | O_TRUNC;
    off_t fpos;
    ssize_t nread, nwrit;
    char buf[SIZEBUF];
    long parsize = getparams(argc, argv);
	
    fprintf(stderr, "Option arguments: %ld\n", parsize);

    int k = optind;
    if (k < argc) {
	fprintf(stderr, "Open stdin: %d %s\n", k, argv[k]);
	f1 = open(argv[k], readflg); //open for reading, position stream at the beginning
	if (f1 < 0) perror("Open input file");
    }
    ++k;

    if(k < argc) {
	fprintf(stderr, "Open stdout: %d %s\n", k, argv[k]);
	f2 = open(argv[k], writflg); // open for write and truncate
	if (f2 < 0) perror("Open output file");
    }

    if ((fpos = lseek(f2, (off_t)0, SEEK_END)) < 0) perror("lseek");
    else fprintf(stderr, "Write position in file %s is: %ld Bytes\n", argv[k], fpos);

    if ((fpos = lseek(f2, parsize, SEEK_END)) < 0) perror("lseek");
    else fprintf(stderr, "moving beyond EOF by %ld Bytes, current position %ld Bytes\n", parsize, fpos);

    while (nread = read(f1, buf, SIZEBUF)) { // get a string from stdin, NULL is returned when EOF or error
        nwrit = 0;
        do {
	    nread -= nwrit;
	    nwrit = write(f2, buf + nwrit, nread); // if write is interrupted, it may return less bytes
	} while (nwrit < nread);                   // in that case the call is restarted until all bytes are written
    }
    
    struct stat finfo;
    if (fstat(f2, &finfo) < 0) perror("fstat"); // fileno extracts file descriptor from FILE
    else {
	fprintf(stderr, "File size: %ld\n", finfo.st_size);
	fprintf(stderr, "Blocks allocated: %ld size on disk: %ld Bytes\n", finfo.st_blocks, finfo.st_blocks * 512);
    }
}



















