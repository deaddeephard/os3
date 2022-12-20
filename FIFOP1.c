#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include<stdlib.h>
#include <netinet/in.h>




static char *generate(size_t length){
	char maines[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	char *rand_s = malloc(30);
	int i=0;
	while(i<length){
	 rand_s[i] =maines[rand() % (int) (sizeof maines-1)];
	i++;
	}
	rand_s[length] = ' ';
	return rand_s;
}



int main()
{
	int descriptor;
	char *rand_s[50];
	int i=0;
	while(i<50){
		rand_s[i] = generate(5);
		if(i<=9){
			sprintf(rand_s[i] , "%s 0%d\n" , rand_s[i] , i);
		}
		else{
			sprintf(rand_s[i] , "%s %d\n" , rand_s[i] , i);
		}
		rand_s[i][8] = '\n';
		rand_s[i][9] = '\0' ;
		i++;
	}
	mkfifo("/tmp/myfifo" , 0666);
	char str1[1000];
	char **mstr = malloc(10000);
	int itr = 0;
	while(itr<50)
	{
		mstr = rand_s;
		descriptor = open("/tmp/myfifo" , O_WRONLY);
		write(descriptor , *(mstr+itr) , strlen(*mstr));
		write(descriptor , *(mstr+itr+1) , strlen(*mstr));
		write(descriptor , *(mstr+itr+2) , strlen(*mstr));
		write(descriptor , *(mstr+itr+3) , strlen(*mstr));
		write(descriptor , *(mstr+itr+4) , strlen(*mstr));
		close(descriptor);
		descriptor =open("/tmp/myfifo" ,O_RDONLY);
		read(descriptor , str1 , sizeof(str1));
		itr =atoi(str1) +1;
		printf("Sent by Client : %s\n" , str1);
		close(descriptor);
	}
	unlink("/tmp/myfifo");
	exit(0);
}

