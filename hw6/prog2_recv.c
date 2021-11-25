/* Created by Artem.Vergazov@skoltech.ru on 25.11.21. 
 * 
 * Prog 2 - receiver: attach to shared segment, read content
 *
 * Syntax of run: ./prog2_send msg
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_KEY	2000 // shm identifier
#define SHM_SIZE 1024 // shm size
#define BUF_SIZE 128

int main() {
    key_t key;
    size_t size;
    int shmid; // shared_block_id
    void *shared_memory;
    char buff[BUF_SIZE];
	
    // Create shared segment.
    if ((shmid = shmget((key_t) SHM_KEY, SHM_SIZE, 0666 | IPC_CREAT)) == -1) { perror("shmid"); exit(1); }
    // Attach memory to the shared segment.
    if ((shared_memory = shmat(shmid, NULL, 0)) == (void *)-1) { perror("shmat"); exit(2); }
	
    // Write in buffer.
    // fgets(buff, BUF_SIZE, stdin);
    // printf("buff is: %s\n", buff);

    // Copy shared memory to the buffer.
    strcpy(buff, shared_memory);
    printf("Attached to shmem key %d, id: %x output: %s\n", SHM_KEY, shmid, buff);

    // Detach shared memory from shmid.
    if ((shmdt(shared_memory) == -1)) { perror("shmdt"); exit(3); }

    // Destroy shared segment.
    if ((shmctl(shmid, IPC_RMID, NULL)) == -1) { perror("shmctl"); exit(4); }
    return 0;
}
