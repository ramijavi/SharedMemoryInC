#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define FOO 4096

int main(){

	int shmId;
	int *shmPtr;
	key_t memKey;
	char *input = malloc(20);

	memKey = ftok("/home/ramijavi/CIS452/lab5", 1);

	if ((shmId =
         	shmget (memKey, FOO, IPC_CREAT | S_IRUSR | S_IWUSR)) < 0) {
        	perror ("Can´t create shared memory segment\n");
        	exit (1);
    	}
    	if ((shmPtr = shmat (shmId, 0, 0)) == (void *) -1) {
        	perror ("Can't attach\n");
        	exit (1);
    	}	
	printf("Successfully created & attached to shared memory segment with ID: %d\n", shmId);


	printf("Input: ");
	scanf("%s", input);
	printf("%s", input);
	
	input = shmPtr;

	// for debugging
	sleep(3);

	if (shmdt (shmPtr) < 0) {
        	perror ("Can´t deattach\n");
        	exit (1);
    	}
    	if (shmctl (shmId, IPC_RMID, 0) < 0) {
        	perror ("Can't deallocate\n");
        	exit (1);
    	}
	printf("Succesfully deattached and deallocated shared memory segment\n");

	return 0;	
}

