
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <time.h>

#define SOCKET_NAME "P1SOCKET.socket"
#define BUFFER_SIZE 10




 void generate(char rand_st[][12]){
	srand (time(NULL));
	for(int j=0;j<=50 ;j++){
		rand_st[j][0] =j;
	}
	for(int i=0;i<=50;i++){
		for(int j=1;j<=10;j++){
			rand_st[i][j] =rand() % 26 + 65;
		}
	}
}

int main(int argc , char* argv[]){
	struct sockaddr_un adr;
	
	int retr;
	int data;
	char buffer[BUFFER_SIZE];

	data = socket(AF_UNIX , SOCK_SEQPACKET , 0);
	if(data ==-1){
		perror("Couldn't create socket");
		exit(EXIT_FAILURE);
	}

	memset(&adr , 0 , sizeof(adr));

	adr.sun_family=AF_UNIX;
	strncpy(adr.sun_path , SOCKET_NAME , sizeof(adr.sun_path)-1);
	retr =connect(data , (const struct sockaddr *) &adr , sizeof(adr));
		
	if(retr==-1){
		perror("Server is down\n");
		exit(EXIT_FAILURE);
	}
	

	
	char rand_S[51][12] ={{0}};
	generate(rand_S);

	
	for(int i=1;i<=50;i++){
		for(int j=0;j<12;j++){
			if(j==0){
				printf("%d" , rand_S[i][j]);
			}
			else printf("%c" , rand_S[i][j]);
		}
		printf("\n");
	}
	if(argc>1){
		strncpy(buffer , "DOWN" , sizeof("DOWN"));
		write(data , buffer , sizeof(buffer));
		close(data);
		exit(EXIT_SUCCESS);
	}

	else{
		int idx=1;
		while(1){
			printf("Sending Strings to P2 from Index %d to %d\n" , idx , idx+4);
			for(int i= idx ;i<idx+5 ;i++){
				retr = write(data , rand_S[i] , strlen(rand_S[i])+1);
				if(retr==-1){
					perror("couldn't write");
				}
	
			}

			retr = read(data , buffer ,sizeof(buffer));
			if(retr == -1){
				perror("read");
				exit(EXIT_FAILURE);

			}

			buffer[sizeof(buffer) -1] =0;
			int idx2 =atoi(buffer);

			printf("MAX ID which is sent back by server = %s\n\n" , buffer);
			if(idx2==50){
				printf("Successfully sent all strings to P2\n");
				strncpy(buffer , "DOWN", sizeof("DOWN"));
				write(data , buffer , sizeof(buffer));
				close(data);
				exit(EXIT_SUCCESS);
				break;
			}
			else{
				idx = idx2 +1;
			}
		}
	}
}














