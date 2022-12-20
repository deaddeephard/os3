#include<stdio.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<string.h>
#include<errno.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<time.h>
#include<stdint.h>
long double t1;

#define SHM_KEY 0x1234


void generate(char rand_str[][12]){
	srand (time(NULL));
	for(int j=0;j<=50;j++){
		rand_str[j][0] =j;
	}
	for(int i=0 ; i<=50 ;i++){
		for(int j=1;j<=10;j++){
			rand_str[i][j] = rand() % 26 +65;
		}
	}
}	



struct shmseg {
	int ackno ;
	int complete;
	char buffer[5][12];
};

int fill_buffer( char * bufptr , int size);

int main(int argc , char *argv[]){
	struct timespec st,end;
	clock_gettime(CLOCK_MONOTONIC , &st);
	int shmid;
	struct shmseg *shmp;
	shmid = shmget(SHM_KEY , sizeof(struct shmseg) , 0644|IPC_CREAT);
	if(shmid == -1){
	perror("Shared memory");
	return 1;
	}

	shmp =shmat(shmid , NULL , 0);
	if(shmp == (void*) -1){
		perror("Shared memory attach");
		return 1;
	}
	int r=1;

	char randstr[51][12] ={{0}};
	generate(randstr);
	int a=0;
	while(r){
		for(int j=0;j<5;j++){

			for(int i=a ; i<a+5 ;i++){
				shmp->buffer[j][0]=randstr[a][0];
				shmp->buffer[j][1]=randstr[a][1];
				shmp->buffer[j][2]=randstr[a][2];
				shmp->buffer[j][3]=randstr[a][3];
				shmp->buffer[j][4]=randstr[a][4];
				shmp->buffer[j][5]=randstr[a][5];
				shmp->buffer[j][6]=randstr[a][6];
				shmp->buffer[j][7]=randstr[a][7];
				shmp->buffer[j][8]=randstr[a][8];
				shmp->buffer[j][9]=randstr[a][9];
				shmp->buffer[j][10]=randstr[a][10];
				shmp->buffer[j][11]=randstr[a][11];		
			}
		}
		shmp->complete=0;
		
		printf("Writing Strings from index %d to %d in shared memory\n" , a,a+5);

		sleep(3);
		
		printf(" The Acknowledge is received: %d\n", shmp->ackno);
		printf("\n");
		if(shmp->ackno==50){
			r=0;
		}
		a+=5;

	}
	printf("Every string is  sent successfully and acknowledge id recieved successfully\n");
	shmp->complete=1;
	clock_gettime(CLOCK_MONOTONIC,&end);
	uint64_t diff =(end.tv_sec-st.tv_sec);
	long double d = (long double)(end.tv_nsec-st.tv_nsec)/1000000000;
	t1 = (long long unsigned int)diff+d;
	printf("Time elapsed is: %Lf \n",(long long unsigned int)diff+d);
	return 0;
}



