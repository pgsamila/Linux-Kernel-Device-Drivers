#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define DEVICENAME "/dev/hello_world"
//#define O_RDWR 2

int ch=0;
char buff[100];
int readResult,writeResult,closeResult,k=0;
size_t openResult;
int main(){

	int t=0;
	
	while(1){
		printf("	 ________________________ \n");
		printf("	|                        |\n");
		printf("	|   SELECT YOUR OPTION   |\n");
		printf("	|                        |\n");
		printf("	| [1]:OPEN THE DEVICE    |\n");
		printf("	| [2]:WRITE TO DEVICE    |\n");
		printf("	| [3]:READ FROM DEVICE   |\n");
		printf("	| [4]:CLOSE THE DEVICE   |\n");
		printf("	| [5]:EXIT THE PROGRAM   |\n");
		printf("	|________________________|\n\n");
		printf("	I need to :");
		scanf("%d",&ch);
		if(ch == 5){
			if(k==1){
				printf("\n	You haven't close your driver!\n");
				printf("\n	If you exit, your driver will close automatically\n");			
			}
			printf("\n	Are you suar you want to exit?\n");
			printf("	     [1]:yes     [2]:no\n");
			scanf("%d",&ch);
			if(ch == 1){
				closeResult = close(openResult);
				if(closeResult < 0){
					printf("\n	 ERROR on closing device");
					return 1;
				}else{
					k=0;
					printf("	-----Device is closing-----\n\n");
				}
					
				
				printf("	----------THANK YOU----------\n");
				break;
			}else{
				ch = 0;
			}
		}else if(ch == 1){// open the file
			openResult = open(DEVICENAME,O_RDWR);
			if(openResult < 0){
				printf("\n	 ERROR on opening device\n");
				return 1;
			}else{
				k=1;
				printf("	-----Device is opening-----\n\n");
			}
		}else if(ch == 2){// write the file
				t=0;
			while(t<3){
				writeResult = write(0,buff,100);
				if(writeResult < 0){
					printf("\n	 ERROR on reading device\n");
					return 1;
				}else{
					printf("	%s\n",buff);
				}
			t++;
			}	
		}else if(ch == 3){// read the file
			readResult = read(openResult,buff,100);
			if(readResult < 0){
				printf("\n	 ERROR on reading device\n");
				return 1;
			}else{
				printf("\n	 %s \n",buff);
			}
		}else if(ch == 4){// close the program
			closeResult = close(openResult);
			if(closeResult < 0){
				printf("\n	 ERROR on closing device");
				return 1;
			}else{
				k=0;
				printf("	-----Device is closing-----\n\n");
			}
		}else{
			if(ch>9){
				ch=0;
			}
			printf("\n	Your have select a wrong option! Try again\n\n");
			ch =0;
		}
	}
	
	
	return 0;
}
