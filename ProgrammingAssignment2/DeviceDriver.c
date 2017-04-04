#include <linux/init.h>           
#include <linux/module.h>         
#include <linux/device.h>         
#include <linux/kernel.h>         
#include <linux/fs.h> 
#include <asm/uaccess.h> 
#define  DEVICE_NAME "DeviceDriver"    ///< The device will appear here
#define  CLASS_NAME  "Device"        ///< The device class 
#define bufferSize 1000

MODULE_LICENSE("GPL");      
MODULE_AUTHOR("Jack Adolfo Allen");    
MODULE_DESCRIPTION("Linux device Driver");  
MODULE_VERSION("1");

// The prototype functions for the character driver -- must come before the struct definition
static int     dev_open(struct inode *, struct file *);
static int     dev_release(struct inode *, struct file *);
static ssize_t dev_read(struct file *, char *, size_t, loff_t *);
static ssize_t dev_write(struct file *, const char *, size_t, loff_t *);

char message[bufferSize];   // string we give to user
short messageSize;
static int Major; 
static int numOpens=0;

static struct class*  ddClass  = NULL; ///< The device-driver class struct pointer
static struct device* ddDevice = NULL; 

static struct file_operations fops =
{
   .open = dev_open,
   .read = dev_read,
   .write = dev_write,
   .release = dev_release,
};

static int init_Driver(void){
    printk(KERN_INFO "Device Module Initialization"); 
   // Register a major number for character device
   Major = register_chrdev(0, DEVICE_NAME, &fops); 
   if(Major<0){
   	// device registration failed
   	 printk(KERN_INFO "Device registration failed at Major %d", Major); 
   	 return Major;
   }

   // register device class and driver
   ddClass=class_create(THIS_MODULE, CLASS_NAME);
   ddDevice=device_create(ddClass,NULL, MKDEV(Major,0),NULL, DEVICE_NAME);

   // device registered
   printk(KERN_INFO "Device has been registered at Major %d", Major); 

   return 0; 	
}


void cleanup(void){
   printk(KERN_INFO "Removing Device Module"); 
   device_destroy(ddClass, MKDEV(Major,0));
   class_unregister(ddClass);
   class_destroy(ddClass);
   unregister_chrdev(Major,DEVICE_NAME); // unregister major number
   printk(KERN_INFO "Device Module has been unregistered from Major %d", Major); 
}

static int dev_open(struct inode *inodep, struct file *filep){
	numOpens++;
	printk(KERN_INFO "Device has been opened %d times", numOpens);
	return 0;

}

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
	buffer="";
}

static ssize_t dev_write(struct file *filep, const char *buffer, size_t len, loff_t *offset){
sprintf(message, "%s(%zu letters)", buffer, len);   // appending received string with its length
  messageSize = strlen(message);                 // store the length of the stored message
   printk(KERN_INFO "EBBChar: Received %zu characters from the user\n", len);
   return len;
}

static int dev_release(struct inode *inodep, struct file *filep){
	numOpens--;
	return 0;

}

module_init(init_Driver); 
module_exit(cleanup); 

