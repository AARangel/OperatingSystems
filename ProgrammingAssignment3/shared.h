extern DEFINE_MUTEX(mutex);


// The prototype functions for the character driver -- must come before the struct definition
extern int     dev_open(struct inode *, struct file *);
extern int     dev_release(struct inode *, struct file *);
extern  ssize_t dev_read(struct file *, char *, size_t, loff_t *);
extern  ssize_t dev_write(struct file *, const char *, size_t, loff_t *);


