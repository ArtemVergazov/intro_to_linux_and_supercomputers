/*
 * First serious assignment in the Linux & Supercomputers class.
 *
 * Revision history:
 * Artem Vergazov       version 1       11.9.21
 *
 */

#include <stdio.h>
#include <stdlib.h> // exit()
#include <unistd.h> // getopt()
#include <string.h>

extern int getopt(int, char *const argv[], const char *);
extern void exit(int);
void usage(char *cmd) {
    printf("%s -s type\n", cmd);
}

int main(int argc, char *argv[]) {
    const char flags[] = "s:";
    int opt;
    int size = 0;
    char *type = NULL;

    extern char *optarg; // getopt points this to the parameter
    extern int optind;
    extern char *optarg;

    while ((opt = getopt(argc, argv, flags)) != -1) {
        switch (opt) {

        case 's':
            type = optarg;
            printf("argument: %s\n", type);
            break;

        default:
            printf("Invalid command flags: %s\n", *argv);
            exit(3);
        }
    }

    if (type == NULL) { usage(argv[0]); exit(0); }

    if (strcmp(type, "char") == 0) size = sizeof(char);
    else if (strcmp(type, "short") == 0) size = sizeof(short);
    else if (strcmp(type, "int") == 0) size = sizeof(int);
    else if (strcmp(type, "float") == 0) size = sizeof(float);
    else if (strcmp(type, "long") == 0) size = sizeof(long);
    else if (strcmp(type, "longlong") == 0) size = sizeof(long long);
    else if (strcmp(type, "double") == 0) size = sizeof(double);
    else size = -1;

    printf("Hello, World! Size of %s: %d\n", type, size);

    if (optind < argc) {
        printf("Other arguments:");
        for (int i = optind; i < argc; ++i)
            printf(" %s", argv[i]);
        printf("\n");
    }

    return 0;
}
