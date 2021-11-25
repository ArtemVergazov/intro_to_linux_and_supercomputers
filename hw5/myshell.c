/* Created by Artem.Vergazov@skoltech.ru on 11.23.21 @11:10PM.
 * Revision 1.0
 *
 * Syntax of run: ./myshell
 *                ls -l <for example>
 *                Ctrl+C
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

#define BUFSIZE 128 // this is the size of the input buffer
#define CMDNAME 32 // max size of a command
#define NUMARGS 8 // max number of arguments
#define FULLCMD 255

int main(int argc, char * argv[]) {
    char buf[BUFSIZE];
    char cmd[BUFSIZE];
    char *cmdargs[NUMARGS] = { NULL };
    int params, totparams;
    int history = 0;
    int nparams = 0;
    int n;
    pid_t pid;
    int status;
    int cpath;
    char fullcmd[FULLCMD];
    char *PATH = getenv("PATH");
    char s1, s2;

    while(1) {
        printf("%-d> ", ++history);
        if (fgets(buf, BUFSIZE, stdin) == NULL) break;
        for(nparams = 0; nparams < NUMARGS; ++nparams) {
            free(cmdargs[nparams]);
            cmdargs[nparams] = 0;
        }

        totparams = params = nparams = 0;
        do {
            n = sscanf(buf + totparams, "%ms %n", &cmdargs[nparams], &params);
            totparams += params;
            ++nparams;
        } while (n > 0);

        // Search for the command with standard PATH.
        cpath = 0;
        do {
            struct stat statbuf;
            int cmdlen = strlen(cmdargs[0]);
            int cp2 = cpath;

            s1 = PATH[cpath];
            s2 = s1;
            while (s2 && (PATH[cpath] != ':')) s2 = PATH[cpath++];
            strncpy(fullcmd, &PATH[cp2], cpath-cp2);
            fullcmd[cpath - cp2] = '/';
                                                                                                                                            22,1          45%
            ++cpath;
            strcpy(&fullcmd[cpath - cp2], cmdargs[0]);
            if (stat(fullcmd, &statbuf) == 0) break;
        } while (s2);

        pid = fork();
        if (pid < 0) { perror("fork"); continue; }
        if (pid == 0) { // child
#ifdef DEBUG
            for(int i = 0; i < nparams; ++i)
                fprintf(stderr, "child cmd %s, %d parameter: %s\n", cmdargs[0], i, cmdargs[i]);
#endif
            if (execve(fullcmd, cmdargs, NULL) < 0) {
                perror("execve");
                exit(EXIT_FAILURE);
            }
        }

        waitpid(-1, &status, 0);
        fprintf(stderr, "status execution of %s %d\n", cmdargs[0], status);

    }
    fprintf(stderr, "all done\n"); // message to the terminal
}
