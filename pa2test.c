#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <string.h>
#define bufflen 1024

int main(int argc, char *argv[]) {
	if(argc != 2){
		printf("Too many or too few command line arguments");
		return -1;	
	}
	if((access(argv[1], F_OK) || access(argv[1], R_OK) || access(argv[1], W_OK)) == 0)
	{
		char *rbuff;
		char statement;
                //FILE* fp = fopen(argv[1], "wr");
		int fd = open(argv[1],O_RDWR); 
		while(1){
			printf("Option (r for read, w for write, s for seek):\n");
			statement = getchar();
			//line = fgets(string, 9, stdin);
			if(statement == 'r'){
				printf("Enter the number of bytes you want to read:\n");
				int readlen;
                                scanf("%d", &readlen);	
				while(getchar() != '\n'); 
				rbuff = malloc(readlen);
				read(fd, rbuff, readlen);
				printf("%s\n", rbuff);
				free(rbuff);
                        }
			else if(statement == 'w'){
				printf("Enter the string you want to write:\n");
                                char wbuff[bufflen];
				scanf("%s", wbuff);
				while(getchar() != '\n');
				write(fd,wbuff,strlen(wbuff));
               		}
			else if(statement == 's'){
				printf("Enter an offset value:\n");
                                int offset;
                                scanf("%d", &offset);
				while(getchar() != '\n'); 
                                int whence;
                                printf("Enter a value for whence (0 for SEEK_SET, 1 for SEEK_CUR, 2 for SEEK_END):\n");
                                scanf("%d", &whence);
				while(getchar() != '\n'); 
                                lseek(fd, offset, whence);

			}
			else if(statement == EOF){
    				close(fd);
				return 0;
			}	
			else {
			}	
		}
	
	}
	else{
		printf("File is not readable, not writable, or doesn't exist");
		return -1;
	}
return 1;
}
	
