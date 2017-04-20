

#include <linux/init.h>           
#include <linux/module.h>         
#include <linux/device.h>         
#include <linux/kernel.h>         
#include <linux/fs.h> 
#include <linux/mutex.h>
#include <asm/uaccess.h> 
#include "shared.c"
#define  DEVICE_NAME "InputDriver"    ///< The device will appear here
#define  CLASS_NAME  "Input"        ///< The device class 
#define bufferSize 1000

MODULE_LICENSE("GPL");      
MODULE_AUTHOR("Jack Adolfo Allen");    
MODULE_DESCRIPTION("Linux device Driver");  
MODULE_VERSION("1");

//DEFINE_MUTEX(mutex);
char message[1000] = {0};

EXPORT_SYMBOL(message);
//EXPORT_SYMBOL(mutex); 

static int Major; 
//char message[bufferSize]={0};
static struct class*  ddClass  = NULL; ///< The device-driver class struct pointer
static struct device* ddDevice = NULL; 

static int dev_open(struct inode *inodep, struct file *filep);
static ssize_t dev_write(struct file *filep, const char *buffer, size_t len, loff_t *offset); 
static int dev_release(struct inode *inodep, struct file *filep);

static struct file_operations fops =
{
   .open = dev_open,
   .write = dev_write,
   .release = dev_release,
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

 static int dev_open(struct inode *inodep, struct file *filep){
	if(mutex_trylock(&mutex)){
		printk(KERN_INFO "Device has been opened\n");
	}
	
	return 0;
}



static ssize_t dev_write(struct file *filep, const char *buffer, size_t len, loff_t *offset){
  sprintf(message, "%s(%zu letters)", buffer, bufferSize); 
	printk(KERN_INFO "%s\n", message);  
   printk(KERN_INFO "Device: Received %zu characters from the user\n", len);
   return len;
}

static int dev_release(struct inode *inodep, struct file *filep){
	// unlock mutex
	mutex_unlock(&mutex);
	printk(KERN_INFO "Device has been closed\n");
	return 0;
}

module_init(init_Driver); 
module_exit(cleanup); 

