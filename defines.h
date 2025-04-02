#ifndef DEFINES_H
#define DEFINES_H

#define MYDEV_NAME "mycdrv"
#define NUM_DEVS 3
#define MAJOR_NUM 500 
#define DEV_PATH "/dev/mydev"
#define RAMDISK_SIZE (size_t) (16*PAGE_SIZE)

#define ASP_IOC_MAGIC 'Z'
#define ASP_CLEAR_BUF _IO(ASP_IOC_MAGIC, 1)
#define ASP_IOC_MAXNR 5


struct asp_mycdev {
    struct cdev dev;
    char *ramdisk;
    struct semaphore sem;
    dev_t dev_no;
    int ramsize;
};

static ssize_t my_write(struct file *file, const char __user * buf, size_t lbuf, loff_t * ppos);
static ssize_t my_read(struct file *file, char __user * buf, size_t lbuf, loff_t * ppos);
static int my_close(struct inode *inode, struct file *file);
static int my_open(struct inode *inode, struct file *file);
static loff_t my_lseek(struct file *flip, loff_t offset, int whence);
static void __exit my_exit(void);
void cleanup_resources(void);
static int __init my_init(void);
static long my_ioctl(struct file *flip, unsigned int cmd, unsigned long arg);

#endif
