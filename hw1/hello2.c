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

extern int getopt(int, char *const[], const char *);
extern void exit(int);

int main(int argc, char *argv[]) {
    const char flags[] = "abc";
    int opt;
    char *name;

    while ((opt = getopt(argc, argv, flags)) != -1) {
        switch (opt) {

        case 'a':
            name = "Alpha";
            break;

        case 'b':
            name  = "Beta";
            break;

        case 'c':
            name = "Charlie";
            break;

        default:
            printf("Invalid command flags: %s\n", *argv);
            exit(3);
        }
    }

    printf("Hello, %s!\n", name);
    return 0;
}
