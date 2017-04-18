extern DEFINE_MUTEX(mutex);


// The prototype functions for the character driver -- must come before the struct definition
extern static int     dev_open(struct inode *, struct file *);
extern static int     dev_release(struct inode *, struct file *);
extern static ssize_t dev_read(struct file *, char *, size_t, loff_t *);
extern static ssize_t dev_write(struct file *, const char *, size_t, loff_t *);


extern struct file_operations fops =
{
   .open = dev_open,
   .read = dev_read,
   .write = dev_write,
   .release = dev_release,
};