/* Created by Artem.Vergazov@skoltech.ru on 11.16.21 @7:44PM
 * 
 * Demonstrate reopen of std I/O streams.
 * Program will copy file in the first argument to the second argument, or std streams.
 * 
 * prog -s <long> in out
 * prog -s <long> <in >out
 * prog -s <long> in       dump file into terminal
 * prog -s <long> in 2>log dump file into terminal and error stream to log
 *
 * Some messages are printed to stderr and can be redirected to stdout with &>
 * (note the arbitrary order of lines fro stderr in redirected output)
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

/* Program definitions
 * put here all explicit numbers and definitions
 */
#define SIZEBUF 128 // size of the input buffer
#define OPTFLAGS "s:" // option flags as compiler symbolic string

/* Global variables
 */
extern int optind;
const char optflags[] = OPTFLAGS;

void usage(int argc, char *argv[]) {
    fprintf(stderr, "Usage: %s [-%c <int>] in out\n", argv[0], optflags[0]);
    exit(0);
}

long getParams(int argc, char *argv[]) {
    const char *flags = optflags;
    char *param = NULL;
    int opt;
    int flag = 0;
    extern char *optarg;
    long parsize = 0;

    while ((opt = getopt(argc, argv, flags)) != -1) {
        if (opt == optflags[0]) { // note: switch needs const in case
            param = optarg;
            parsize = atoll(param);
            // break; should NOT break the while loop to collect all flags/parameters
        } else usage(argc, argv);
    }
    return parsize;
}

int main(int argc, char *argv[]) {
    FILE *f1 = stdin;
    FILE *f2 = stdout;
    char buf[SIZEBUF]; // use preprocessor defined constants
    long parsize = getParams(argc, argv);
    int i;

    fprintf(stderr, "Option arguments: %ld\n", parsize);

    int k = optind;
    if (k < argc) {
        fprintf(stderr, "Open stdin: %d %s\n", k, argv[k]);
        f1 = freopen(argv[k], "r", stdin); printf("\n"); // open for reading; position stream at the beginning
        if (f1 == NULL) perror("Open file on stdin");
    }
    ++k;
    
    if (k < argc) {
        fprintf(stderr, "Open stdout: %d %s\n", k, argv[k]);
        f2 = freopen(argv[k], "w", stdout); printf("\n"); // w - open for writing and truncate to zero length
        if (f2 == NULL) perror("Open file on stdout"); // create if does not exist
    }
    
    while (fgets(buf, SIZEBUF, f1)) { // get a string from stdin, NULL is returned when EOF or error
        buf[strlen(buf) - 1] = '\0'; // remove the trailing return-character '\n'
        puts(buf); // TODO
    }
    fprintf(stderr, "All done\n"); // message to the terminal
}

