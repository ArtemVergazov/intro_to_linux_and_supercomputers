/* Created by Artem.Vergazov@skoltech.ru
 * Revision 1.0 11.17.21 22:29
 *
 * Summary:
 * the program counts words in text file by mapping out the text file and using pointers
 * using hashtables
 * similar to wc -w <filename> but may differ due to different definition of word
 *
 * Syntax of run:
 * ./prog6 <filename>
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
#define HASHTAB_SIZE ((1 << 9) - 1)
#define HASHSEED 0x12345678
#define NUMBUCKETS 10
#define MAXWORD_LEN 20

typedef unsigned int uint32_t;

typedef struct hashdata {
        long count;
        long colis;
        char word[MAXWORD_LEN];
} HashdataType;

static long _collisions = 0;
static HashdataType *hashtabs[NUMBUCKETS] = {0};

uint32_t MurmurOAAT_32(const char *str, int strlen, uint32_t h) {
        int i;
        for (i = 0; i < strlen; ++i) {
                h ^= *str++;
                h *= 0x5bd1e995;
                h ^= h >> 15;
        }
        return h;
}

HashdataType *allhash() {
        HashdataType *hashtable;

        if ((hashtable = (HashdataType *)mmap(
                NULL, sizeof(HashdataType) * HASHTAB_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON, -1, (off_t)0
        )) == NULL) {
                perror("mmap hashdata");
                exit(4);
        } else memset(hashtable, (int)0, sizeof(HashdataType) * HASHTAB_SIZE);
        return hashtable;
}

void hashfree() {
        int i;
        for (i = 0; i < NUMBUCKETS; ++i) if (hashtabs[i] != NULL) free(hashtabs[i]);
}

void printinfo() {
        HashdataType *htab;
        int i, j, newl;
        int sparce[NUMBUCKETS] = {0};

        printf("%32s Freq Colis\n", "word");
        for (i = 0; i < HASHTAB_SIZE; ++i) {
                newl = 0;
                for (j = 0; j < NUMBUCKETS; ++j) {
                        if ((htab = hashtabs[j]) == NULL) break;
                        if (htab[i].count > 0 ) {
                                printf("%16s    %d      %d", htab[i].word, htab[i].count, htab[i].colis);
                                ++sparce[j];
                                newl =  1;
                        }
                }
                if (newl) printf("\n");
        }
        for (j = 0; j < NUMBUCKETS; ++j) {
                printf(" %.2f ", (double)sparce[j] / HASHTAB_SIZE);
        }
        printf("collisions: %ld\n", _collisions);
}

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
        while (!isalnum(*p1) && *p1 ) ++p1;
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
        HashdataType *htab;
        long word_count = 0;
        uint32_t hashvalue;
        int hindex;

        p2 = data;

        do {
                int wsize;
                int i;
                p1 = p2;
                p2 = get_word(&p1);
                if (p2 == NULL) break;
                wsize = ((p2 - p1) < MAXWORD_LEN ? p2 - p1 : MAXWORD_LEN - 1);

                ++word_count;

                hashvalue = MurmurOAAT_32(p1, wsize, HASHSEED);
                hindex = hashvalue & HASHTAB_SIZE;

                for(i = 0; i < NUMBUCKETS; ++i) {
                        htab = hashtabs[i];
                        if (htab == NULL) htab = hashtabs[i] = allhash();

                        if (htab[hindex].count == 0) {
                                strncpy(htab[hindex].word, p1, wsize);
                                htab[hindex].word[wsize] = '\0';
                                ++htab[hindex].count;
                                break;
                        }
                        else {
                                if (strncmp(htab[hindex].word, p1, wsize) == 0) {
                                        ++htab[hindex].count;
                                        break;
                                }
                                else {
                                        ++htab[hindex].colis;
                                }
                        }
                }
                if (i == NUMBUCKETS) ++_collisions;

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

        printinfo();

        munmap(data, alloc_bytes);
        exit(0);
}
