/* Created by Artem.Vergazov@skoltech.ru on 11.25.21 @10:18PM.
 *
 * parent recieves data from stdin
 * child recieves data from the parent and prints it to stdout
 *
 * Syntax of run: ./prog1b
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

main() {
    int pfd[2];
    // pfd[0] - read from
    // pfd[1] - write to

    if (pipe(pfd) == -1) { perror("pipe"); exit(1); }
	
    int pid = fork();
    if (pid < 0) { perror("fork"); exit(2); }
    else if (pid) close(pfd[0]); // read side, parent writing
    else close(pfd[1]);	// write side, child reading

    while(1) {
        int inchar;
	int n;
	if (pid < 0) { perror("fork"); exit(3); }
	else if (pid) { // parent code
	    inchar = getchar();
	    // scanf("%d", &inchar);
	    printf("parent %6d send %c n=%d\n", pid, inchar, n);
	    sleep(1);
	    if ((n = write(pfd[1], &inchar, sizeof(inchar))) == -1) { perror("write"); exit(4); }
	} else { // child code
	    if ((n = read(pfd[0], &inchar, sizeof(inchar))) == -1) { perror("read"); exit(5); }
	    printf("child %6d	got %c n=%d\n", pid, inchar, n);
	}
    }
    return 0;
}

