#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>
 
#define BUFFER_LENGTH 256               ///< The buffer length (crude but fine)
static char receive[BUFFER_LENGTH];     ///< The receive buffer from the LKM
 
int main(){
   int ret, fd;
   char stringToSend[BUFFER_LENGTH];

   printf("Starting device test code example...\n");
   fd = open("/dev/DeviceDriver", O_RDWR); 
         
   if (fd < 0){
      perror("Failed to open the device\n");
      return errno;
   }

   int value; 
   printf("0\twrite\n1\tread\n2\texit\n"); 
   while(1){

	if(1)
		scanf("%d", &value);
	switch(value){
	   case 0: // write
		printf("Type into Device: ");
		scanf("%[^\n]%*c", stringToSend);            
   		ret = write(fd, stringToSend, strlen(stringToSend)); // Send the string to the LKM
   		if (ret < 0){
			perror("Failed to write the message to the device.");
			return errno;
   		}
		break;
	   case 1: 
		printf("Reading from Device:\n");
		ret = read(fd, receive, BUFFER_LENGTH);
		if (ret < 0){
      			perror("Failed to read the message from the device.");
			return errno;
		}
		printf("%s\n", receive);
		break; 
	   case 2: 
		printf("Program terminating\n");
		return 0;
		break;  
	} 
   }

}
