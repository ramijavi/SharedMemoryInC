#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <string.h>
#include "data.h"

#define MESSAGE_COUNT 30

int main () {
    int shmId;
    char *shmPtr;
	key_t memKey;
    struct data d;

	memKey = ftok("test", 1);

    int totalMem = sizeof(struct data) * MESSAGE_COUNT;
    if ((shmId =
        shmget (memKey, totalMem, S_IRUSR)) < 0) {
        perror ("Can´t access shared memory address\n");
        exit (1);
    }

    if ((shmPtr = shmat (shmId, 0, 0)) == (void *) -1){
        perror ("Can't attach\n");
        exit (1);
    }
//    printf ("Successfuly found & attached to shared memory segment with ID: %d\n", shmId);
   
    int memPos = 0; 
    int assignedIndex = 0;
    struct data *shared_translate = ((struct data *)shmPtr)+memPos;
    int i;
    for(i = 0; i < READER_NUM; i++) {
        if(shared_translate->readers[i] == 1) {
            assignedIndex = i;
        }
    }
    while(1) {
        shared_translate = ((struct data *)shmPtr)+memPos;
        if(shared_translate->readers[assignedIndex] == 1) {
            printf("Message: %s", shared_translate->message);
            if(strncmp(shared_translate->message, "quit\n", 5) == 0)
                break;
            shared_translate->readers[assignedIndex] = 0;
            memPos++;
        }
        if(memPos > MESSAGE_COUNT)
            memPos = 0;

        shared_translate = ((struct data *)shmPtr)+memPos;
        while(shared_translate->readers[assignedIndex] != 1);
    }

    if (shmdt (shmPtr) < 0) {
        perror ("Can´t deattach\n");
        exit (1);
    }

    //printf("Successfully deattached from shared memory segment\n");

    return 0;
}
