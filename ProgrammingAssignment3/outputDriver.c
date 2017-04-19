
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

static int Major; 
static struct class*  ddClass  = NULL; ///< The device-driver class struct pointer
static struct device* ddDevice = NULL; 

MODULE_LICENSE("GPL");      
MODULE_AUTHOR("Jack Adolfo Allen");    
MODULE_DESCRIPTION("Linux device Driver");  
MODULE_VERSION("1");

 struct file_operations fops =
{
   .read = dev_read,
};

int init_Driver(void){
	mutex_init(&mutex);
    printk(KERN_INFO "Device Module Initialization\n"); 
   // Register a major number for character device
   Major = register_chrdev(0, DEVICE_NAME, &fops); 
   if(Major<0){
   	// device registration failed
   	 printk(KERN_INFO "Device registration failed at Major %d\n", Major); 
   	 return Major;
   }

   // register device class and driver
   ddClass=class_create(THIS_MODULE, CLASS_NAME);
   ddDevice=device_create(ddClass,NULL, MKDEV(Major,0),NULL, DEVICE_NAME);

   // device registered
   printk(KERN_INFO "Device has been registered at Major %d\n", Major); 

   return 0; 	
}


void cleanup(void){
	// destroy mutex
	mutex_destroy(&mutex);
   printk(KERN_INFO "Removing Device Module\n"); 
   device_destroy(ddClass, MKDEV(Major,0));
   class_unregister(ddClass);
   class_destroy(ddClass);
   unregister_chrdev(Major,DEVICE_NAME); // unregister major number
   printk(KERN_INFO "Device Module has been unregistered from Major %d\n", Major); 
}


char message[bufferSize]={0};

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

module_init(init_Driver); 
module_exit(cleanup); 

