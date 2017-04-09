
#include <linux/init.h>           
#include <linux/module.h>         
#include <linux/device.h>         
#include <linux/kernel.h>         
#include <linux/fs.h> 
#include <linux/mutex.h>
#include <asm/uaccess.h> 
#define  DEVICE_NAME "OutputDriver"    ///< The device will appear here
#define  CLASS_NAME  "Output"        ///< The device class 
#define bufferSize 1000

MODULE_LICENSE("GPL");      
MODULE_AUTHOR("Jack Adolfo Allen");    
MODULE_DESCRIPTION("Linux device Driver");  
MODULE_VERSION("1");

static DEFINE_MUTEX(mutex);
// The prototype functions for the character driver -- must come before the struct definition
static int     dev_open(struct inode *, struct file *);
static int     dev_release(struct inode *, struct file *);
static ssize_t dev_read(struct file *, char *, size_t, loff_t *);
static ssize_t dev_write(struct file *, const char *, size_t, loff_t *);

char message[bufferSize];   // string we give to user
short messageSize;
static int Major; 

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
 mutex_init(&mutex);

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

static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset){
	
}

// need to souround with mutexs
static ssize_t dev_write(struct file *filep, const char *buffer, size_t len, loff_t *offset){
   sprintf(message, "%s(%zu letters)", buffer, bufferSize);   
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

