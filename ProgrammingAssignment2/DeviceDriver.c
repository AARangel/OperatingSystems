// see www.tldp.org/LDP/lkmpg/2.6/html/lkmpg.html

#include <linux/init.h>           
#include <linux/module.h>         
#include <linux/device.h>         
#include <linux/kernel.h>         
#include <linux/fs.h>      

// The prototype functions for the character driver -- must come before the struct definition
static int     dev_open(struct inode *, struct file *);
static int     dev_release(struct inode *, struct file *);
static ssize_t dev_read(struct file *, char *, size_t, loff_t *);
static ssize_t dev_write(struct file *, const char *, size_t, loff_t *);

static int Major; 
static int numOpens=0;

static struct file_operations fops =
{
   .open = dev_open,
   .read = dev_read,
   .write = dev_write,
   .release = dev_release,
};

static int init_Driver(void){
   printk(KERN_INFO "Device Module has been initialized"); 
   // Register a major number for character device
   Major = register_chrdev(0, "Driver Program For OS", &fops); 
   if(Major<0){
   	// device registration failed
   	 printk(KERN_INFO "Device registration failed at Marjor %d", Major); 
   	 return Major;
   }
   // device registered
   printk(KERN_INFO "Device has been registered at Marjor %d", Major); 
   return 0; 	
}


void cleanup(void){
   printk(KERN_INFO "Removing Device Module"); 
   unregister_chrdev(Major, "Driver Program For OS"); 
   printk(KERN_INFO "Device Module has been unregistered from Major %d", Major); 
}


static int dev_open(struct inode *inodep, struct file *filep){
	numOpens++;
	printk(KERN_INFO "Device has been opened %d times", numOpens);
	return 0;

}

static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset){
   return 0; 
}

static ssize_t dev_write(struct file *filep, const char *buffer, size_t len, loff_t *offset){
  return 0; 
}

static int dev_release(struct inode *inodep, struct file *filep){
	numOpens--;
	//module_put(THIS_MODULE);
	return 0;

}

module_init(init_Driver); 
module_exit(cleanup); 

