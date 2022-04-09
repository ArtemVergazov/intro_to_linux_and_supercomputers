/* Created by Artem.Vergazov@skoltech.ru on 11.23.21 @11:03PM.
 * Revision 1.0
 *
 * Syntax of run: ./myfile filename
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

#define BUFSIZE	128 // this is the size of the output buffer
#define NLINES  50  // number of lines to write to output

int main(int argc, char *argv[]) {
    char buf[BUFSIZE];
    char *filename;
    FILE * f1;
    int n = NLINES;
    int numchars;
    char *process;
    pid_t pid, ppid;
    useconds_t delay_microsec;
    int linepos;
    int i;
	
    if (argc > 1) filename = argv[1];
    else exit(0); 

    if ((f1 = fopen(filename, "w")) < 0) { perror(filename); exit(1); }
        fprintf(stderr, "opening file %s\n", filename);
	
	// setlinebuf(f1); // alternatively: flush the buffer
	// if (setvbuf(f1, NULLm _IONBF, 0) < 0) perror("setvbuf"); // unbuffered
	
	pid = fork();
	
	if (pid < 0) { perror("fork"); exit(2); }
	if (pid > 0) { process = "parent"; delay_microsec = 500000; linepos = NLINES; }
	else { process = "child";  delay_microsec = 650000; linepos = 1; }

	pid = getpid(); ppid = getppid();

	for (i = 0; i < NLINES; ++i) {

            fprintf(stderr, "now printing %s\n", process); // message to the terminal
            numchars = snprintf(buf,BUFSIZE,"pid=%6d ppid=%6d %6s       Here is line %3d\n",
                pid, ppid, process, i); // note COPY-ON-WRITE pages
	    // if (linepos)
	    fseek(f1, (long)(numchars * (i + 1) * linepos), SEEK_SET); // MT_SAFE
		
	    usleep(delay_microsec);
		
	    if (fputs(buf, f1) < 0) { perror("fputs"); exit(3); }
            fflush(f1); // flush the buffer
	}
	wait(NULL);
	fprintf(stderr, "all done %s\n", process); // message to the terminal
}       
