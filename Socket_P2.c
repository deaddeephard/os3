#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCKET_NAME "P1SOCKET.socket"
#define BUFFER_SIZE 12

int main(int agrc , char *argv[]){
	struct sockaddr_un server;
	int flg=0;
	int con_soc;
	int retr;
	int idx=0;
	int data;
	char buffer[BUFFER_SIZE];

	con_soc = socket(AF_UNIX ,SOCK_SEQPACKET , 0 );
	if(con_soc == -1){
		perror("Socket couldn't be made");
		exit(EXIT_FAILURE);
	}

	memset(&server, 0 , sizeof(server));
	server.sun_family =AF_UNIX;
	strncpy(server.sun_path , SOCKET_NAME , sizeof(server.sun_path)-1);

	retr = bind(con_soc , (const struct sockaddr *) &server, sizeof(server));
	if(retr == -1){
		perror("bind");
		exit(EXIT_FAILURE);
	}

	retr = listen(con_soc,100);
	if(retr ==-1){
		perror("listen");
		exit(EXIT_FAILURE);
	}

	while(1){
		data = accept(con_soc , NULL , NULL);
		if(data ==-1){
			perror("couldn't accept");
			exit(EXIT_FAILURE);
		}

		idx=0;
		int idx2 =idx;
	 	while(1){
			retr = read(data , buffer , sizeof(buffer));
			if(retr==-1){
				perror("couldn't read");
				exit(EXIT_FAILURE);
			}

			buffer[sizeof(buffer)-1]=0;
			idx2 =buffer[0];
			if(!strncmp(buffer, "DOWN" , sizeof(buffer))){
				flg =1;
				break;
			}
			printf("THE STRING SENT IS:");
			for(int i=1;i<11;i++){
				printf("%c" , buffer[i]);
			}
			printf("\n");
			sprintf(buffer , "%d" , idx2);
			printf("THE ID SENT BY CLIENT IS =%s\n" , buffer);
			if(idx2== idx+5){
				idx=idx2;
				retr = write(data , buffer , sizeof(buffer));
			}
			if(idx2 >=50){
				break;
			}
		}
		
		close(data);

		if(flg){
			printf("SERVER IS SHUTTING DOWN");
			close(con_soc);
			unlink(SOCKET_NAME);
			exit(EXIT_SUCCESS);
			break;
		}
		if(retr ==-1){
			perror("write");
			exit(EXIT_FAILURE);
		}

	}

	return 0;

}

