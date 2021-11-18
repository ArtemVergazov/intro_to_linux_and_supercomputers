/* Created by Artem.Vergazov@skoltech.ru
 * Revision 11.17.21 21:59
 * LabX4
 *
 * Summary:
 * the program allocates memory using mmap depending on number of provided elements
 *
 * Syntax of run:
 * ./prog -s <N>
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h> // LONG_MAX
#include <sys/sysinfo.h>
#include <errno.h> // the error number and codes system
#include <sys/mman.h>

#define MiB 1.0 / (1024 * 1024)
#define GiB 1.0 / (1024 * 1024 * 1024)
#define OPTFLAGS "s:"

unsigned long long freeram() {
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
        const long pagesize = sysconf(_SC_PAGESIZE);

        int i;
        long num_elements = getparams(argc, argv);

        unsigned long long freememory = freeram();
        unsigned long long freeleft;
        unsigned long long usedmem;

        typedef struct mys {
                double x;
                double y;
        } MysType;

        size_t mys_bytes = num_elements * sizeof(MysType);
        MysType *data;
        long pagesdef = 1 + (mys_bytes + pagesize - 1) / pagesize;

        printf("Number of elements: %d avaliable memory: %.2f MB\n", num_elements, (double)freememory * MiB);

        mys_bytes = pagesdef * pagesize;

        if ((mys_bytes <= 0) || (mys_bytes > freememory)) {
                fprintf(stderr, "illegal number of elements: %d, FreeMem: %u\n", num_elements, freememory);
                exit(4);
        }
        data = (MysType *)mmap(NULL, mys_bytes, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON, -1, (off_t)0);

        if (data == NULL) perror("mmap");
        else memset(data, (int)0xFF, mys_bytes);

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

        munmap(data, mys_bytes);
}
 