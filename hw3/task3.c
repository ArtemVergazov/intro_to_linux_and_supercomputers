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
    FILE *f1 = stdin;
    FILE *f2 = stdout;
    char buf[SIZEBUF];
    long parsize = getparams(argc, argv);

    fprintf(stderr, "Option arguments: %ld\n", parsize);

    int k = optind - 1;
    if (k < argc) {
        fprintf(stderr, "Open stdin: %d %s\n", k, argv[k]);
        f1 = freopen(argv[k], "r", stdin); printf("\n"); //open for reading, position stream at the beginning
        if (f1 == NULL) perror("Open file on stdin");
    }
    ++k;

    if(k < argc) {
        fprintf(stderr, "Open stdout: %d %s\n", k, argv[k]);
        f2 = freopen(argv[k], "w", stdout); printf("\n"); // w - open for write and truncate to zero length
        if (f2 == NULL) perror("Open file on stdout"); // create if does not exist
    }

    int flagCapitalsPresent = 0;
    int flagLowerPresent = 0;
    while (fgets(buf, SIZEBUF, f1)) { // get a string from stdin, NULL is returned when EOF or error
        flagCapitalsPresent = 0;
        flagLowerPresent = 0;
        buf[strlen(buf) - 1] = '\0'; // remove the trailing return-character '\n'
        int i;
        for(i = 0; i < strlen(buf); ++i) {
            if (isupper(buf[i]) != 0) flagCapitalsPresent = 1;
            if (islower(buf[i]) != 0) {
                flagLowerPresent = 1;
                break;
            }
        }
        if (flagCapitalsPresent == 1 && flagLowerPresent != 1) puts(buf);
    }
    fprintf(stderr, "All done\n"); // message to the terminalf
}
