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

static struct file_operations fops =
{
   open = dev_open,
   read = dev_read,
   write = dev_write,
   release = dev_release,
};

static init_module(void){
   printk(KERN_INFO "Device Module has been initialized"); 
   // Register a major number for character device
   Major = register_chrdev(999, "Driver Program For OS", &fops); 
   printk(KERN_INFO "Device has been registered at Marjor %d", Major); 
   return 0; 	
}

void cleanup_module(void){
   printk(KERN_INFO "Removing Device Module"); 
   unregister_chrdev(999, "Driver Program For OS"); 
   printk(KERN_INFO "Device Module has been unregistered from Major %d", Major); 
}

static int dev_open(struct inode *inodep, struct file *filep){

}

static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset){
   
}

static ssize_t dev_write(struct file *filep, const char *buffer, size_t len, loff_t *offset){
  
}

static int dev_release(struct inode *inodep, struct file *filep){

}
