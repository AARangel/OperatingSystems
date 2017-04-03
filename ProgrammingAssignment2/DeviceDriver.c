

#include <linux/init.h>           
#include <linux/module.h>         
#include <linux/device.h>         
#include <linux/kernel.h>         
#include <linux/fs.h> 
#include <asm/uaccess.h>     

// The prototype functions for the character driver -- must come before the struct definition
static int     dev_open(struct inode *, struct file *);
static int     dev_release(struct inode *, struct file *);
static ssize_t dev_read(struct file *, char *, size_t, loff_t *);
static ssize_t dev_write(struct file *, const char *, size_t, loff_t *);

#define DEVICE_NAME "Driver Program For OS"
#define CLASS "DEVICE_DRIVER"
#define bufferSize 1000

int Major = 0; 
int numberOpens=0;
// int bufferSize=1000; // bytes
char message[bufferSize];   // string we give to user
short messageSize;

static struct class* deviceClass=NULL;
static struct  device* driverDevice=NULL;

static struct file_operations fops =
{
   .open = dev_open,
   .read = dev_read,
   .write = dev_write,
   .release = dev_release,
};

int init_module(void){
   printk(KERN_INFO "Device Module Initialization"); 
   // Register a major number for character device
   Major = register_chrdev(0, DEVICE_NAME, &fops); 
   if(Major<0){
   	// device registration failed
   	 printk(KERN_INFO "Device registration failed at Major %d", Major); 
   	 return Major;
   }
   // device registered
   printk(KERN_INFO "Device has been registered at Major %d", Major); 

   // register device class and driver
   deviceClass=class_create(THIS_MODULE, CLASS);

   driverDevice=device_create(deviceClass,NULL, MKDEV(Major,0),NULL, DEVICE_NAME);
   return 0; 	
}

void cleanup_module(void){
   int ret = 0;

   printk(KERN_INFO "Removing Device Module"); 
   device_destroy(deviceClass, MKDEV(Major,0));
   class_unregister(deviceClass);
   class_destroy(deviceClass);

   
   unregister_chrdev(Major,DEVICE_NAME); // unregister major number
   if(ret<0){
   		// problem unregistering device
   		printk(KERN_INFO "problem unregistering device");
   }
   printk(KERN_INFO "Device Module has been unregistered from Major %d", Major); 
}

static int dev_open(struct inode *inodep, struct file *filep){
	numberOpens++;
	printk(KERN_INFO "Device has been opened %d times", numberOpens);
	return 0;

}

// read in only buffer size bits
static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset){
	int errors;
	int bytesRead;

   errors = 0;
   bytesRead = 0;
	errors=copy_to_user(buffer,message, bufferSize);

	if(errors==0){
		// successfully read all the the bits
		printk(KERN_INFO " Sent %d characters to user\n", bufferSize);
      return 0;
	}
	else{
		//not all the bits read

		printk(KERN_INFO " %d bytes were not read in only have %d bytes available\n", errors, bufferSize);
      return errors;
	}
   
}

static ssize_t dev_write(struct file *filep, const char *buffer, size_t len, loff_t *offset){
   return 0;
  
}

static int dev_release(struct inode *inodep, struct file *filep){
	numberOpens--;
	//module_put(THIS_MODULE);
	return 0;

}
