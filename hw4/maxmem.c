/* Created by Artem.Vergazov@skoltech.ru
 * Revision 1.0 18.11.2021 21:39
 * LabX-4
 *
 * Summary:
 * the program allocates maximum supported memory and fills it up with zeros until killed by system
 *
 * Launch syntax:
 * ./maxmem
 * ./maxmem &> log &; watch free -h
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

unsigned long long freeram() { // get the avaliable memory from system
    struct sysinfo info;
    unsigned long long ram = 0;

    if (sysinfo(&info) < 0) perror("sysinfo");
    else ram = info.freeram;
    return ram;
}

int main() {
    size_t mys_bytes = LONG_MAX;
    char *data;
    long ptr = NULL;
    long onegig = 1024 * 1024 * 1024;

    unsigned long long freememory = freeram();

    fprintf(stderr, "Avaliable memory: %.2f GiB\n", (double)freememory * GiB);
    fprintf(stderr, "Asking allocation max size: %lld %.2f GB\n", mys_bytes, mys_bytes * GiB);

    if (malloc(mys_bytes) == NULL) perror("malloc");
/* an alternative way to look at the errors:
 */
    fprintf(stderr, "error number %d text: %s\n", errno, strerror(errno));

    while ((data = (char *)malloc(mys_bytes)) == NULL) mys_bytes >>= 1;
    fprintf(stderr, "successfuly allocared %lld %.2f GB\n", mys_bytes, mys_bytes * GiB);

    do {
        memset(data + ptr, (int)0, (size_t)onegig);
        sleep(1);
        ptr += onegig;
        fprintf(stderr, "memory %10X - %10X initialized %10lld %.2f GB\n", data + ptr - onegig, data + ptr - 1, ptr, ptr * GiB);
    } while (ptr < mys_bytes);

    free(data);
}
