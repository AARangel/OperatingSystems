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
int numOpens=0;

static struct file_operations fops =
{
   open = dev_open,
   read = dev_read,
   write = dev_write,
   release = dev_release,
};

static int dev_open(struct inode *inodep, struct file *filep){
   numOpens++;
   printk(KERN_INFO "EBBChar: Device has been opened %d time(s)\n", numOpens);
   return 0;
}

static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset){
   
}

static ssize_t dev_write(struct file *filep, const char *buffer, size_t len, loff_t *offset){
  
}

static int dev_release(struct inode *inodep, struct file *filep){
   printk(KERN_INFO " Device successfully closed\n");
   return 0;
}
