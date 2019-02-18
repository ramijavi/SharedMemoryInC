#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define FOO 4096

int main () {

    	int shmId;
    	char *shmPtr;
	key_t memKey;

	memKey = ftok("/home/ramijavi/CIS452/lab5", 1);
	// for debugging
	sleep(1);
    	if ((shmId =
         	shmget (memKey, FOO, S_IRUSR | S_IWUSR)) < 0) {
       		perror ("Can´t access shared memory address\n");
        	exit (1);
    	}

    	if ((shmPtr = shmat (shmId, 0, 0)) == (void *) -1){
        	perror ("Can't attach\n");
        	exit (1);
    	}

    	printf ("Successfuly found & attached to shared memory segment with ID: %d\n", shmId);
	
    	if (shmdt (shmPtr) < 0) {
        	perror ("Can´t deattach\n");
        	exit (1);
    	}

	printf("Successfully deattached from shared memory segment\n");

    	return 0;
}
