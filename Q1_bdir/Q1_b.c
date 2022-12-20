#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<semaphore.h>

sem_t chopsticks[5];
sem_t sauce_b;

void * func(void*);

int main(){
	pthread_t philosopher[5];
	int a[5];
	
	for(int i=0 ;i<5;i++){
		sem_init(&chopsticks[i],0,1);
	}
	sem_init(&sauce_b,0,2);
	for(int i=0;i<5;i++){
		a[i]=i;
		pthread_create(&philosopher[i] , NULL , func , (void*)&a[i]);
	}
	for(int i=0;i<5;i++){
		pthread_join(philosopher[i] , NULL);
	}
	return 0;
}
	

void * func(void* n){
	while(1){
		int phil = *(int*)n;
		int num;
		int k = (phil+1)%5;
		if(phil==4){
			sem_wait(&chopsticks[k]);
			printf("\n..picks chopstick %d",k);
			sem_wait(&chopsticks[phil]);
			printf("\n..picks chopstick %d" ,phil);
		}
		else{
			sem_wait(&chopsticks[phil]);
			printf("\n..picks chopstick %d" , phil);
			sem_wait(&chopsticks[k]);
			printf("\n..picks chopstick %d" , k);
		}
		
		sem_getvalue(&sauce_b , &num);
		printf("\nValue of the bowl semaphore is :- %d\n",num);

		sem_wait(&sauce_b);
		printf("Philosopher %d has picked the bowl\n" , phil);
		
		printf("Philosopher %d started eating\n",phil);
		sleep(1);
		printf("Philosopher %d has finished eating\n",phil);
		
		sem_post(&sauce_b);
		printf("Philosopher %d has left the bowl\n" , num);
	
		sem_getvalue(&sauce_b , &num);
		printf("\nValue of bowl semaphore :- %d\n",num);
		
		sem_post(&chopsticks[k]);
		printf("\n..puts down chopsticks %d\n",k);
		
		sem_post(&chopsticks[phil]);
		printf("\n..puts down chopsticks %d\n" ,phil);
	}
}
	
