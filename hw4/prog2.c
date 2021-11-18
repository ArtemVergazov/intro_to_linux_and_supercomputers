/* Created by Artem.Vergazov@skoltech.ru
 * Revision 1.0 11.17.2021 21:51
 * LabX4
 *
 * Summary:
 * the program allocates memory using malloc depending on number of provided elements
 *
 * Syntax of run:
 * ./prog -s <N>
 *
 * To allocate 6 GB, N ~ 393000000
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h> // get LONG_MAX
#include <sys/sysinfo.h>
#include <errno.h> // the error number and codes system

#define MiB 1.0 / (1024 * 1024)
#define GiB 1.0 / (1024 * 1024 * 1024)
#define OPTFLAGS "s:"

unsigned long long freeram() { // get the available memory from system
    struct sysinfo info;
    unsigned long long ram = 0;

    if (sysinfo(&info) < 0) perror("sysinfo");
    else ram = info.freeram;
    return ram;
}

extern int optind;
const char optflags[] = OPTFLAGS;

void usage(int argc, char *argv[]) {
    fprintf(stderr, "usage: %s [-%c <long>] in out/n", argv[0], optflags[0]);
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
        if (opt == optflags[0]) {
            param = optarg;
            parsize = atoll(param);
        }
        else usage(argc, argv);
    }
    return parsize;
}

int main(int argc, char *argv[]) {
    int i;
    long num_elements = getparams(argc, argv);

    unsigned long long freememory = freeram();
    unsigned long long freeleft;
    unsigned long long usedmem;

    typedef struct mys {
        double x;
        double y;
    } MysType;

    size_t mys_bytes;
    MysType *data;

    printf("Number of elements: %d avaliable memory: %.2f MB\n", num_elements, (double)freememory * MiB);

    mys_bytes = num_elements * sizeof(MysType);
    if ((mys_bytes <= 0) || (mys_bytes > freememory)) {
    fprintf(stderr, "illegal number of elements: %d, FreeMem: %u\n", num_elements, freememory);
    exit(4);
    }
    data = (MysType *)malloc(mys_bytes);
    if (data == NULL) perror("malloc");

    for(i = 0; i < num_elements; ++i) {
        data[i].x = 1.0;
        data[i].y = 2.0;
    }

    freeleft = freeram();
    usedmem = freememory - freeleft;

    fprintf(
        stderr, "memory allocation size %lld Bytes %.1f MiB done. Used memory: %llu Bytes %.1f MiB\n",
        mys_bytes, (double)mys_bytes * MiB, usedmem, (double)usedmem * MiB
    );
    //sleep(10);
    free(data);
}
           