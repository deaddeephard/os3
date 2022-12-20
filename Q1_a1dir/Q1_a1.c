#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>
#include<sched.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>

void *func(void *);

pthread_mutex_t chopstick[5];

int main(){
	int i,a[5];
	pthread_t philosopher[5];

	for( i=1;i<=5;i++){
		pthread_mutex_init(&chopstick[i],NULL);
	}
	for( i=1;i<=5;i++){
		a[i]=i;
		pthread_create(&philosopher[i] , NULL , func, (void*)&a[i]);
	}
	for( i=1;i<=5;i++){
		pthread_join(philosopher[i] , NULL);
	}
	for( i=1 ;i<=5;i++){
		pthread_mutex_destroy(&chopstick[i]);
	}
	return 0;
}

void *func(void * num){
	while(1){
	
		int phil = *(int*)num;
	
		printf("\n Philosopher %d has entered room" , phil);
		int k = (phil+1)%5;
		
		if(phil==4){
			pthread_mutex_lock(&chopstick[k]);
			printf("\n .. picks chopstick %d" ,k);
			
			pthread_mutex_lock(&chopstick[phil]);
			printf("\n ..picks chopstick %d" ,phil);
		}
		else{
			pthread_mutex_lock(&chopstick[phil]);
			printf("\n ..picks chopstick %d" , phil);
			
			pthread_mutex_lock(&chopstick[k]);
			printf("\n ..picks chopstick %d" , k);
		}


		printf("\n Philosopher %d is eating " , phil);
		sleep(2);
		printf("\n Philopher %d finished eating" , phil);	


		pthread_mutex_unlock(&chopstick[k]);
		printf("\n ..puts back chopstick %d" ,k);

		pthread_mutex_unlock(&chopstick[phil]);
		printf("\n ..puts back chopstick %d" ,phil);
	}	

}

