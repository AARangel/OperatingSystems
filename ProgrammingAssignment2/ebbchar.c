
 

 
#include <linux/init.h>           
#include <linux/module.h>         
#include <linux/device.h>         
#include <linux/kernel.h>         
#include <linux/fs.h>            
 
 int bufferSize;
int    deviceNumber;                  ///< Stores the device number -- determined automatically
char   message[256] = {0};           ///< Memory for the string that is passed from userspace
int  size_of_message;              ///< Used to remember the size of the string stored
int    numberOpens = 0;              ///< Counts the number of times the device is opened
struct class*  deviceClass  = NULL; ///< The device-driver class struct pointer
static struct device* deviceDriver = NULL; ///< The device-driver device struct pointer
 
// The prototype functions for the character driver -- must come before the struct definition
int     dev_open(struct inode *, struct file *);
int     dev_release(struct inode *, struct file *);
ssize_t dev_read(struct file *, char *, size_t, loff_t *);
ssize_t dev_write(struct file *, const char *, size_t, loff_t *);
 
*/

read back in fifo 
write - remove from buffer as read back out
 read- store only up to buffer size else print not enough room
*/

// reguster devic
e log device number
int init_module( void){
      printk(KERN_INFO " install kernel module")
}
// de register device
void cleanup_module(void){
   printk(KERN_INFO "end kernel module")
}
int dev_open(struct inode *inodep, struct file *filep){
   numberOpens++;
   printk(KERN_INFO "EBBChar: Device has been opened %d time(s)\n", numberOpens);
   return 0;
}
 

ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset){
   int error_count = 0;
   // copy_to_user has the format ( * to, *from, size) and returns 0 on success
   error_count = copy_to_user(buffer, message, size_of_message);
 
 
   if (error_count==0){            // if true then have success
      printk(KERN_INFO "DeviceDriver: Sent %d characters to the user\n", size_of_message);
      return (size_of_message=0);  // clear the position to the start and return 0
   }
   else {
      printk(KERN_INFO "DeviceDriver: Failed to send %d characters to the user\n", error_count);
      return -EFAULT;              // Failed -- return a bad address message (i.e. -14)
   }
}
 

ssize_t dev_write(struct file *filep, const char *buffer, size_t len, loff_t *offset){
   sprintf(message, "%s(%zu letters)", buffer, len);   // appending received string with its length
   size_of_message = strlen(message);                 // store the length of the stored message
   printk(KERN_INFO "Received %zu characters from the user\n", len);
   return len;
}
 

int dev_release(struct inode *inodep, struct file *filep){
   printk(KERN_INFO "Device successfully closed\n");
   return 0;
}


