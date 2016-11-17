#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>

#include "semaph.h"

#define DEVICENAME "/dev/hello_world"

#define RED     "\x1b[31m"
#define YELLOW  "\x1b[33m"
#define GREEN   "\x1b[32m"
#define BLUE    "\x1b[34m"
#define RESET   "\x1b[0m"

size_t fd;

enum status {
	OPENED,
	CLOSED
};

enum status file_status = CLOSED;

int exit_software(enum status *file_status, size_t *fd, int *ch);
void device_menu(int *ch);
void device_close(enum status *file_status,size_t *fd);
void device_open(enum status *file_status, size_t *fd );
void led_off(enum status *file_status,size_t *fd);
void led_on(enum status *file_status,size_t *fd);
void device_write(enum status *file_status,size_t *fd,char* msg);
void device_read(enum status *file_status,size_t *fd,char* msg);


int main(int argc, char *argv[]){
    int ch=0;
    char msg;
    int ret =0;
    while(1){
    	/* load device menu */
		device_menu(&ch);
		
		switch(ch){

			case 1:
				device_open(&file_status, &fd);
			break;

			case 2:
				/* Turon on lights */
				led_on(&file_status,&fd);
			break;
			/*
			case 3:
				/ * Turon off lights * /
				led_off(&file_status,&fd);
			break;*/

			case 4:
				/* Write data */
				device_write(&file_status, &fd, &msg);
			break;

			case 5:
				/* Read data */
				device_read(&file_status, &fd, &msg);
			break;

			case 6:
				device_close(&file_status, &fd);
			break;

			case 7:
				ret = exit_software(&file_status, &fd, &ch);
			break;

			default:
				ch = 0;
			break;
		}

		if(1==ret){
			break;
		}

    } 
    return 0;
}


/* This function will be the main menu */
void device_menu(int *ch){
	printf( BLUE "	 ________________________ \n");
	printf( BLUE "	|                        |\n");
	printf( BLUE "	|   " YELLOW "SELECT YOUR OPTION" BLUE "   |\n");
	printf( BLUE "	|                        |\n");
	printf( BLUE "	|  " GREEN "[1]:OPEN THE DEVICE" BLUE "   |\n");
	printf( BLUE "	|  " GREEN "[2]:TURN ON LIGHT" BLUE "     |\n");
	/*printf( BLUE "	|  " GREEN "[3]:TURN OFF LIGHT" BLUE "    |\n");*/
	printf( BLUE "	|  " GREEN "[4]:WRITE DATA" BLUE "        |\n");
	printf( BLUE "	|  " GREEN "[5]:READ DATA" BLUE "         |\n");
	printf( BLUE "	|  " GREEN "[6]:CLOSE THE DEVICE" BLUE "  |\n");
	printf( BLUE "	|  " RED   "[7]:EXIT THE PROGRAM" BLUE "  |\n");
	printf( BLUE "	|________________________|\n\n");
	printf( RESET "	I need to :");
	scanf("%d",ch);
	
}

/* This function will off led * /
void led_off(enum status *file_status,size_t *fd){			
	if(OPENED==*file_status){
		if(-1 == ioctl(*fd, IOCTL_LEDOFF,2)){
			printf(RED"\n	 [ ERROR ] "YELLOW" turning on light\n"RESET);
		}
	}else{
		printf(RED"\n	 [ warning ] "YELLOW	"	device is not open\n\n"	RESET);
	}
}*/

/* This function will on led */
void led_on(enum status *file_status,size_t *fd){			
	if(OPENED==*file_status){
		if(-1 == ioctl(*fd, IOCTL_GLEDON,1)){
			printf(RED"\n	 [ ERROR ] "YELLOW" turning on light\n"RESET);
		}
	}else{
		printf(RED"\n	 [ warning ] "YELLOW	"	device is not open\n\n"	RESET);
	}
}

/* This function will write data to device */
void device_write(enum status *file_status,size_t *fd,char *msg){			
	if(OPENED==*file_status){
		printf(GREEN "Enter your data :\n");
		scanf("%s" RESET ,msg);
		if(-1 == ioctl(*fd, IOCTL_WRITE,msg)){
			printf(RED"\n	 [ ERROR ] "YELLOW" writing device\n"RESET);
		}	
	}else{
		printf(RED"\n	 [ warning ] "YELLOW	"	device is not open\n\n"	RESET);
	}
}

/* This function will read data from device */
void device_read(enum status *file_status,size_t *fd,char *msg){			
	if(OPENED==*file_status){
		if(-1 == ioctl(*fd, IOCTL_READ,msg)){
			printf(RED"\n	 [ ERROR ] "YELLOW" reading device\n"RESET);
		}
		printf(GREEN"Your data : %s\n\n" RESET,msg );
	}else{
		printf(RED"\n	 [ warning ] "YELLOW	"	device is not open\n\n"	RESET);
	}

}

/* This function will close the device */
void device_close(enum status *file_status,size_t *fd){
	int close_result;
	if(OPENED==*file_status){
		close_result = close(*fd);
		if(close_result < 0){
			printf(RED	"\n	 ---==[ ERROR ]==---\n"	RESET);
		}else{
			*file_status=CLOSED;
			printf(GREEN	"	---==[ device closed ]==---\n\n"	RESET);
		}
	}else{
		printf(YELLOW	"	device is not open\n\n"	RESET);
	}

}

/* This function will open the device */
void device_open(enum status *file_status, size_t *fd ){
	if(OPENED == *file_status){
		printf(RED"\n	 [ ERROR ] "YELLOW" You have already open the device\n"RESET);
	}else{
		*fd = open(DEVICENAME,O_RDWR);
		if(fd < 0){
			printf(RED"\n	 [ ERROR ] "YELLOW" opening device\n"RESET);
		}else{ 
			*file_status=OPENED;
			printf(GREEN"	---==[ Device is opening ]==---\n\n"RESET);
		}
	}
}

/* The function will exit the user software */
int exit_software(enum status *file_status,size_t *fd,int *ch){
	if(OPENED == *file_status){
		printf(YELLOW"\n	You haven't close your driver!\n"RESET);
		printf(RED"\n	If you exit, your driver will close automatically\n"RESET);			
	}

	printf(YELLOW"\n	Are you suar you want to exit?\n"RESET);
	printf(RED"	     [1]:yes"GREEN"     [2]:no\n"RESET);
	scanf("         %d",ch);

	if(OPENED == *file_status && 1 == *ch){ /* file is open & want to close */
		device_close(file_status,fd);
		printf(YELLOW"	------===="GREEN"[ THANK YOU ]"YELLOW"====------\n\n"RESET);
	return 1;
	}else if (1 == *ch){
		/* file close want to close */
		printf(YELLOW"	------===="GREEN"[ THANK YOU ]"YELLOW"====------\n\n"RESET);	
		return 1;
	}else{			
		/* do not close */
		return 0;
	}
}
