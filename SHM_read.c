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
		perror("Shared Memory");
		return 1;
	}
	shmp =shmat(shmid , NULL , 0);
	if(shmp==(void*)-1){
		perror("Shared Memory attach");
		return 1;
	}
	while(shmp->complete!=1){
		for(int i=0;i<5;i++){
			for(int j=0;j<12;j++){
				if(j==0){
					 printf("%d" , shmp->buffer[i][j]);
				}
				
				else{
					 printf("%c" , shmp->buffer[i][j]);
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

