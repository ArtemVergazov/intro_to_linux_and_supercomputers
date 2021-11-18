/* Created by Artem.Vergazov@skoltech.ru
 * Revision 1.0 11.17.21 22:05
 *
 * Summary:
 * the program counts words in text file by mapping out the text file and using pointer similar to wc -w <filename>
 *
 * Syntax of run:
 * ./prog4 <filename>
 */


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h> // LONG_MAX
#include <sys/sysinfo.h>
#include <errno.h> // the error number and codes system
#include <sys/mman.h>
#include <fcntl.h>

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

char *get_word(char **ps) {
        char *p1, *p2;

        p1 = *ps;
        while (!isalnum(*p1) && *p1) ++p1;
        *ps = p2 = p1;

        while (isalnum(*p2) && *p2) ++p2;

#ifdef DEBUG
        {
                char word[50];
                int wsize = p2 - p1;
                strncpy(word, p1, wsize);
                word[wsize] = '\0';
                printf("word found: %d -%s-\n", wsize, word);
        }
#endif
        return p2;
}

long process_words(char *data) {
        char *p1, *p2;
        long word_count = 0;

        p2 = data;

        do {
                p1 = p2;
                p2 = get_word(&p1);
                if (p2 == NULL) break;
                ++word_count;
        } while (p2 > p1);

        return word_count;
}

int main(int argc, char *argv[]) {
        const long pagesize = sysconf(_SC_PAGESIZE);

        char *data;
        struct stat file_status;
        int fd;
        char *file_name;
        size_t alloc_bytes;
        long pagesdef;

        long word_count = 0;

        if (argc <= 1) usage(argc, argv);
        else file_name = argv[1];

        if ((fd = open(file_name, O_RDONLY)) < 0) { perror("open"); exit(1); }
        if (fstat(fd, &file_status) < 0) { perror("fstat"); exit(1); }
        pagesdef = 1 + (file_status.st_size + pagesize - 1) / pagesize;

        alloc_bytes = pagesdef * pagesize;
        data = (char *)mmap(NULL, alloc_bytes, PROT_READ, MAP_PRIVATE, fd, (off_t)0);
        if (data == NULL) { perror("mmap"); exit(1); }
        else close(fd);

        word_count = process_words(data);

        printf("number of words in file %s (size %ld): %ld\n", file_name, file_status.st_size, word_count);

        munmap(data,alloc_bytes);
        exit(0);
}
            