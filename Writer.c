#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include "data.h"

#define MESSAGE_COUNT 30

int main(int argc, char *argv[]){
	key_t memKey;
	int shmId;
    	int *shmPtr;

	memKey = ftok("test", 1);
    	int totalMem = sizeof(struct data) * MESSAGE_COUNT;
	
	if ((shmId =
         	shmget (memKey, totalMem, IPC_CREAT | S_IRUSR | S_IWUSR)) < 0) {
        	perror ("Can´t create shared memory segment\n");
        	exit (1);
    	}
    	if ((shmPtr = shmat (shmId, 0, 0)) == (void *) -1) {
       		perror ("Can't attach\n");
        	exit (1);
    	}	
	printf("Successfully created & attached to shared memory segment with ID: %d\n", shmId);

    	//clear the memory so we don't potentially get old values
    	int i;
    	for(i = 0; i < totalMem; i++) {
        	*(((char *)shmPtr)+i) = 0;
    	}

    	//ensure that it by very random chance doesn't already have "quit" already
    	int memPos = 0;
    	//struct data shmData;
    	while(1) {
        	struct data *shared_translate = ((struct data *)shmPtr)+memPos;
        	printf("Input: ");
        	fgets(shared_translate->message, MESSAGE_SIZE, stdin);
        	for(i = 0; i < READER_NUM; i++)
            		shared_translate->readers[i] = 1;

        	if(strncmp(shared_translate->message, "quit\n", 5) == 0)
           		break;
        	memPos++;
        	//loop in shared memory buffer
        	if(memPos > MESSAGE_COUNT)
            		memPos = 0;

        	shared_translate = ((struct data *)shmPtr)+memPos;
		//waiting for readers to read before continuing
        	if(shared_translate->readers[0] == 1 || shared_translate->readers[1] == 1)
            		printf("waiting for readers to catch up");
        	while(shared_translate->readers[0] == 1 || shared_translate->readers[1] == 1);
    	}
 
	if (shmdt (shmPtr) < 0) {
        	perror ("Can´t deattach\n");
        	exit (1);
	}
    	if ((shmctl (shmId, IPC_RMID, 0)) < 0) {
        	perror ("Can't deallocate\n");
        	exit (1);
    	}

	return 0;	
}

