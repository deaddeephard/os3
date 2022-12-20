#include<stdio.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/types.h>
#include<string.h>
#include<errno.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>


#define SHM_KEY 0x1234

struct shmseg{
	int ackn;
	int complete;
	char buffer[5][12];
};

int main(int argc , char *argv[]){
	sleep(3);
	int shmid;
	struct shmseg *shmp;
	shmid = shmget(SHM_KEY , sizeof(struct shmseg) , 0644|IPC_CREAT);
	if(shmid==-1){
		perror("Shared Memory error happened");
		return 1;
	}
	shmp =shmat(shmid , NULL , 0);
	if(shmp==(void*)-1){
		perror("Shared Memory attach error happened");
		return 1;
	}
	while(shmp->complete!=1){
		for(int p=0;p<5;p++){
			for(int k=0;k<12;k++){
				if(k==0){
					 printf("%d" , shmp->buffer[p][k]);
				}
				
				else{
					 printf("%c" , shmp->buffer[p][k]);
				}
			}
			printf("\n");
		}
		printf("Reading Strings in shared memory and sending acknowledge\n");
		shmp->ackn = (int)shmp->buffer[4][0];
		sleep(3);
	}
	printf("\nAll Strings recieved and read\n");

	if(shmdt(shmp) ==-1){
	perror("shmdt");
	return 1;
	}
	return 0;
}

