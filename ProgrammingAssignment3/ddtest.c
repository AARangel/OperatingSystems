#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>
 
#define BUFFER_LENGTH 256               ///< The buffer length (crude but fine)
static char receive[BUFFER_LENGTH];     ///< The receive buffer from the LKM
 
int main(){
   int ret, fd, fr;
   char stringToSend[BUFFER_LENGTH];

   printf("Starting device test code example...\n");

   fd = open("/dev/InputDriver", O_RDWR); 

   fr = open("/dev/OutputDriver", O_RDWR); 
         
   if (fd < 0){
      perror("Failed to open the  input device\n");
      return errno;
   }

   
   if (fr < 0){
      perror("Failed to open the output device\n");
      return errno;
   }


   printf("Type into Device: ");
   scanf("%[^\n]%*c", stringToSend);            
   ret = write(fd, stringToSend, strlen(stringToSend)); // Send the string to the LKM
   if (ret < 0){
	perror("Failed to write the message to the device.");
	return errno;
   }

   printf("Reading from Device:\n");
   ret = read(fr, receive, BUFFER_LENGTH);
   if (ret < 0){
   	perror("Failed to read the message from the device.");
	return errno;
   }
   printf("%s\n", receive);

   printf("Program terminating\n");
   return 0;
	

}
