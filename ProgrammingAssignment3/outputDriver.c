
#include <linux/init.h>           
#include <linux/module.h>         
#include <linux/device.h>         
#include <linux/kernel.h>         
#include <linux/fs.h> 
#include "shared.h"

#include <asm/uaccess.h> 
#define  DEVICE_NAME "OutputDriver"    ///< The device will appear here
#define  CLASS_NAME  "Output"        ///< The device class 
#define bufferSize 1000

MODULE_LICENSE("GPL");      
MODULE_AUTHOR("Jack Adolfo Allen");    
MODULE_DESCRIPTION("Linux device Driver");  
MODULE_VERSION("1");




char message[bufferSize];   // string we give to user
short messageSize;
static int Major; 

 struct file_operations fops =
{
   .read=dev_read,
};




 ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset){
      int errors;
   int bytesRead;
   
   errors = 0;
   bytesRead = 0;
   errors=copy_to_user(buffer,message, bufferSize);

   if(errors==0){
      // successfully read all the the bits
      
      if(bufferSize < len)
         printk(KERN_INFO " Sent %d characters to user\n", bufferSize);
      else 
         printk(KERN_INFO " Sent %d characters to user\n", len);

            return 0;
   }
   else{
      //not all the bits read
      printk(KERN_INFO " %d bytes were not read in only have %d bytes available\n", errors, bufferSize);
            return errors;
   }
   buffer="";
	
}


EXPORT_SYMBOL(dev_read);
