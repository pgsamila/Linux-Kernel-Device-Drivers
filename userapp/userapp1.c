#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define DEVICENAME "/dev/hello_world"
#define RED     "\x1b[31m"
#define YELLOW  "\x1b[33m"
#define GREEN   "\x1b[32m"
#define BLUE    "\x1b[34m"
#define RESET   "\x1b[0m"

int ch=0;
char buff[100];
int read_result,write_result,close_result;
size_t fd;

enum status {
	OPENED,
	CLOSED
};

enum status file_status = CLOSED;

int exit_software(enum status file_status, int close_result, size_t fd, int ch);
void device_menu(int ch);
void device_close(enum status file_status, int close_result, size_t fd);
void wrong_input(int ch);
void device_read(enum status file_status, int read_result,char *buff,size_t fd);
void device_write(enum status file_status, int write_result, char *buff, size_t fd);
void device_open(enum status file_status, size_t fd );

int main(){
	int ret=0;
	while(1){
		/* load device menu */
		device_menu(ch);
		scanf("%d",&ch);
		switch(ch) {
			/* check user input */
			case 0:
				scanf("%d",&ch);
			break;
			/* open the device */
			case 1:
				device_open(file_status, fd);
				file_status=OPENED;
			break;
			/* write to device */
			case 2:
				device_write(file_status, write_result, buff, fd);
			break;
			/* read form device */
			case 3:
				device_read(file_status, read_result, buff, fd);
			break;
			/* close the device */
			case 4:
				device_close(file_status, close_result, fd);
				ch =0;
			break;
			/* exit the software */
			case 5:
				ret = exit_software(file_status, close_result, fd, ch);	
			break;
			/* wrong user input */
			default:
				wrong_input(ch);
			break;					
		}
		if(1==ret){
			break;
		}
	}
	return 0;
}

/* This funtion will close the device */
void device_close(enum status file_status,int close_result,size_t fd){
	close_result = close(fd);
	if(OPENED==file_status){
		if(close_result < 0){
			printf(RED	"\n	 [ ERROR ]\n"	RESET);
		}else{
			file_status=CLOSED;
			printf(GREEN	"	-----[ device closed ]-----\n\n"	RESET);
		}
	}else{
		printf(GREEN	"	device is not open\n\n"	RESET);
	}
	
}

/* This function will open the device */
void device_open(enum status file_status, size_t fd ){
	if(OPENED == file_status){
		printf(RED"\n	 [ ERROR ] "YELLOW" You have already open the device\n"RESET);
	}else{
		fd = open(DEVICENAME,O_RDWR);
		if(fd < 0){
			printf(RED"\n	 [ ERROR ] "YELLOW" opening device\n"RESET);
		}else{
			file_status=OPENED;
			printf(GREEN"	-----[ Device is opening ]-----\n\n"RESET);
		}
	}
}

/* This function will write data on the device */
void device_write(enum status file_status,int write_result, char *buff, size_t fd){
	if(CLOSED==file_status){
		printf(RED"	[ ERROR ] "YELLOW" please open the device first\n"RESET);
	}else{
		printf(GREEN"\nEnter your data:"RESET);	
		scanf("%s",buff);
		write_result = write(fd,buff,100);
		if(write_result < 0){
			printf(RED"\n 	 [ ERROR ] "YELLOW" reading device\n"RESET);
		}else{
			printf("	%s\n",buff);
		}
	}
}

/* This function will read the device data */
void device_read(enum status file_status,int read_result,char *buff,size_t fd){
	if(CLOSED==file_status){
		printf(RED"	[ ERROR ] "	YELLOW" please open the device first\n"RESET);
	}else {
		read_result = read(fd,buff,100);
		if(read_result < 0){
			printf(RED"\n	 [ ERROR ]" 	YELLOW	" reading device\n"RESET);
		}else{
			printf("\n	 %s \n",buff);
		}
	}
}

/* This function will check for wrong user inputs */
void wrong_input(int ch){
	if(9 < ch){
		ch = 0;
	}
	printf(RED	"\n	Your have select a wrong option! Try again\n\n"	RESET);
	ch = 0;
}

/* This function will create the user menu */
void device_menu(int ch){
	printf( BLUE "	 ________________________ \n");
	printf( BLUE "	|                        |\n");
	printf( BLUE "	|   ");
	printf( YELLOW 	"SELECT YOUR OPTION");
	printf( BLUE "   |\n");
	printf( BLUE "	|                        |\n");
	printf( BLUE "	|  ");
	printf( GREEN	"[1]:OPEN THE DEVICE");
	printf( BLUE "   |\n");
	printf( BLUE "	|  ");
	printf( GREEN	"[2]:WRITE TO DEVICE");
	printf( BLUE "   |\n");
	printf( BLUE "	|  ");
	printf( GREEN	"[3]:READ FROM DEVICE");
	printf( BLUE "  |\n");
	printf( BLUE "	|  ");
	printf( GREEN	"[4]:CLOSE THE DEVICE");
	printf( BLUE "  |\n");
	printf( BLUE "	|  ");
	printf( RED	"[5]:EXIT THE PROGRAM");
	printf( BLUE "  |\n");
	printf( BLUE "	|________________________|\n\n");
	printf( RESET "	I need to :");
	
}

/* The function will exit the user software */
int exit_software(enum status file_status,int close_result,size_t fd,int ch){
	if(OPENED == file_status){
		printf(YELLOW"\n	You haven't close your driver!\n"RESET);
		printf(RED"\n	If you exit, your driver will close automatically\n"RESET);			
	}
	printf(YELLOW"\n	Are you suar you want to exit?\n"RESET);
	printf(RED"	     [1]:yes"GREEN"     [2]:no\n"RESET);
	scanf("%d",&ch);
	if(OPENED == file_status && 1 == ch){ /* file is open & want to close */
		device_close(file_status,close_result,fd);
		printf(YELLOW"	----------"GREEN"[ THANK YOU ]"YELLOW"----------\n"RESET);
		return 1;
	}else if (1 == ch){	/* file close want to close */
		printf(YELLOW"	----------"GREEN"[ THANK YOU ]"YELLOW"----------\n"RESET);	
		return 1;
	}else{			/* do not close */
		return 0;
	}
}

