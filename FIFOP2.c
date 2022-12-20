#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
	int files_d1;
	char *myfifo = "/tmp/myfifo";
	mkfifo(myfifo ,0666);
	int i= 0;
	char arr1[1000] = {0}, *arr2 =malloc(1000);
	while(i<49)
	{
		files_d1 = open(myfifo , O_RDONLY);
		read(files_d1 , arr1 ,1000);
		printf("Sent to Server: %d\n" ,i);
		printf("%s" , (arr1));
		close(files_d1);
		int len = strlen(arr1);

		const char *idx = &arr1[len-3];
		i = atoi(idx);
		files_d1 = open(myfifo , O_WRONLY);
		sprintf(arr2 , "%d" , i );
		write(files_d1 , arr2 , strlen(arr2)+1);
		close(files_d1);
	}
	unlink(myfifo);
	exit(0);
}

