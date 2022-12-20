#include <stdio.h>
#include <stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>
#include<sched.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<fcntl.h>

sem_t chopstick[5];
void* func(void *);

int main(){
	int i , a[5];
	pthread_t philosopher[5];
	for(i=0;i<5;i++){
		sem_init(&chopstick[i],0,1);
	}
	for(i=0;i<5;i++){
		a[i] =i;
		pthread_create(&philosopher[i] , NULL , func , (void*)&a[i]);
	}
	for(i=0;i<5;i++){
		pthread_join(philosopher[i],NULL);
	}
}

void *func(void *n){
	while(1){
	
		int phil =*(int*)n;
		printf("\nPhilosopher %d has started thinking" , phil);
		int k =(phil+1)%5;
		if(phil==4){
			sem_wait(&chopstick[k]);
			printf("\n ..picks chopstick %d" ,k);
			sem_wait(&chopstick[phil]);
			printf("\n..picks chopstick %d" , phil);
		}
		else{
			sem_wait(&chopstick[phil]);
			printf("\n..picks chopstick %d" , phil);
			sem_wait(&chopstick[k]);
			printf("\n..picks chopstick %d" , k);
		}
	
		printf("\nPhilospher %d is eating" , phil);
		sleep(2);
		printf("\nPhilospher %d has finished eating" ,phil);

		sem_post(&chopstick[k]);
		printf("\n ..putting back chopstick %d" , k);
	
		sem_post(&chopstick[phil]);
		printf("\n ..putting back chopstick %d" , phil);

	}


}


							
