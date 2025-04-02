#include <linux/module.h>	/* for modules */
#include <linux/fs.h>		/* file_operations */
#include <linux/uaccess.h>	/* copy_(to,from)_user */
#include <linux/init.h>		/* module_init, module_exit */
#include <linux/slab.h>		/* kmalloc */
#include <linux/cdev.h>		/* cdev utilities */
#include <linux/semaphore.h>

#include "defines.h"

int majorno = 500;
int numdevices = 3;
int size = 16*4096;
module_param(numdevices, int, S_IRUGO);
module_param(majorno, int, S_IRUGO);
module_param(size, int, S_IRUGO);
int count = 1;
int clean_count = 0;

static struct class *dev_class;
static struct asp_mycdev *mycdev;

static struct file_operations my_fops = {
    .owner = THIS_MODULE,
    .open = my_open,
    .read = my_read,
    .write = my_write,
    .release = my_close,
    .unlocked_ioctl = my_ioctl,
    .llseek = my_lseek
};

static long my_ioctl(struct file *flip, unsigned int cmd, unsigned long arg) {

    if (_IOC_TYPE(cmd) != ASP_IOC_MAGIC) return -ENOTTY;
    if (_IOC_NR(cmd) > ASP_IOC_MAXNR) return -ENOTTY;

    struct asp_mycdev *dev = flip->private_data;

    if (down_interruptible(&dev->sem)) return -ERESTARTSYS;

    switch (cmd) {
        case ASP_CLEAR_BUF:
            {

                char *tmpptr = dev->ramdisk;
                for (int i = 0; i < dev->ramsize; i++) {

                    *tmpptr = 0;
                    tmpptr++;
                }

                flip->f_pos = 0;

                break;
            }
        default:
            up(&dev->sem);
            pr_err("Failed command\n");
            return -1;
    }

    up(&dev->sem);
    return 0;
}

static int __init my_init(void) {


    mycdev = kmalloc(numdevices * sizeof(struct asp_mycdev), GFP_KERNEL); 

    dev_class = class_create("my_dev_class");
    if (IS_ERR(dev_class)) {

        unregister_chrdev(majorno, MYDEV_NAME);
        pr_err("Failed to register device class\n");
        return PTR_ERR(dev_class);
    }

    dev_t first = MKDEV(majorno, 0);
    register_chrdev_region(first, numdevices, MYDEV_NAME);
    for (int i = 0; i < numdevices; i++) {//create nodes loop

       mycdev[i].dev_no = MKDEV(majorno, i); 

       struct device *dev = device_create(dev_class, NULL, mycdev[i].dev_no, NULL, "mycdrv%d", i);
       if (IS_ERR(dev)) {

           clean_count = i;
           pr_err("Error creating node for %d\n", i);
           goto cleanup; //cleanup handling for device removal
        }
        mycdev[i].ramsize = size; 
        mycdev[i].ramdisk = kmalloc(size, GFP_KERNEL);
	sema_init(&mycdev[i].sem, 1);
	if (mycdev[i].ramdisk == NULL) {

           clean_count = i;
		goto cleanup;
	}

        cdev_init(&mycdev[i].dev, &my_fops);
        cdev_add(&mycdev[i].dev, mycdev[i].dev_no, count);
    }

	return 0;
cleanup:
    
    for (int i = clean_count; i >= 0; i--) {
        
        cdev_del(&mycdev[i].dev);
	kfree(mycdev[i].ramdisk);
        device_destroy(dev_class, mycdev[i].dev_no);
    }
    class_destroy(dev_class);
    kfree(mycdev);

    return -1;
}

void cleanup_resources(void) {

    unregister_chrdev_region(mycdev[0].dev_no, numdevices);
    for (int i = 0; i < numdevices; i++) {

        cdev_del(&mycdev[i].dev);
        device_destroy(dev_class, mycdev[i].dev_no);
        kfree(mycdev[i].ramdisk);
    }
    class_destroy(dev_class);
    kfree(mycdev);
}

static void __exit my_exit(void)
{
    cleanup_resources();
	pr_info("\nShreyas Ganesh device unregistered\n");
}

static loff_t my_lseek(struct file *flip, loff_t offset, int whence) {

    struct asp_mycdev *dev = flip->private_data;

    if (down_interruptible(&dev->sem)) return -ERESTARTSYS;

    switch (whence) {
        case SEEK_SET:
            {
		if (offset < 0) {

			up(&dev->sem);
			return -EINVAL;
		}
                if ((dev->ramsize > offset)) {

                    flip->f_pos = offset;
                } else {

                    char *tmp;
                    if ((tmp = krealloc(dev->ramdisk, offset, GFP_KERNEL)) == NULL) {

                        pr_err("Realloc failed in offset change\n");
			    up(&dev->sem);
			return -ENOMEM;
                    }
                        dev->ramdisk = tmp;
                    
                    char *tmpptr = dev->ramdisk + dev->ramsize;
                    for (int i = dev->ramsize; i < offset; i++) {// '0'ing out

                        *tmpptr = 0;
                        tmpptr++;
                    }

                    dev->ramsize = offset;
                    flip->f_pos = offset;
                }
                break;

            }
        case SEEK_CUR:
            {
		if ((offset + flip->f_pos) < 0) {

			up(&dev->sem);
			return -EINVAL;
		}
                if (((offset + flip->f_pos) < dev->ramsize)) {

                    flip->f_pos += offset;
                } else {

                    char *tmp;
                    if ((tmp = krealloc(dev->ramdisk, offset + flip->f_pos, GFP_KERNEL)) == NULL) {
                        pr_err("Realloc failed in offset change\n");
			    up(&dev->sem);
			return -ENOMEM;
                    }
                        dev->ramdisk = tmp;

                    char *tmpptr = dev->ramdisk + dev->ramsize;
                    for (int i = dev->ramsize; i < offset + flip->f_pos; i++) {// '0'ing out

                        *tmpptr = 0;
                        tmpptr++;
                    }

                    dev->ramsize = offset + flip->f_pos;
                    flip->f_pos += offset;
                }

                break;
            }
        case SEEK_END:
            {
                
		if ((offset + dev->ramsize) < 0) {

			up(&dev->sem);
			return -EINVAL;
		}
		char *tmp;
                if ((tmp = krealloc(dev->ramdisk, offset + dev->ramsize, GFP_KERNEL)) == NULL) {

                    pr_err("Realloc failed in offset change\n");
		    up(&dev->sem);
			return -ENOMEM;
                }
			dev->ramdisk = tmp;

                char *tmpptr = dev->ramdisk + dev->ramsize;
                for (int i = 0; i < offset; i++) {// '0'ing out

                    *tmpptr = 0;
                    tmpptr++;
                }

                dev->ramsize += offset;
                flip->f_pos += offset;

                break;
            }
        default:
            up(&dev->sem);
            return -EINVAL;
    }
    up(&dev->sem);
    return flip->f_pos;
}


static int my_open(struct inode *inode, struct file *file)
{
    struct asp_mycdev *cdev = container_of(inode->i_cdev, struct asp_mycdev, dev);
    file->private_data = cdev;

	pr_info(" OPENING device: %s:\n\n", MYDEV_NAME);
	return 0;
}

static int my_close(struct inode *inode, struct file *file)
{
	pr_info(" CLOSING device: %s:\n\n", MYDEV_NAME);
	return 0;
}

static ssize_t my_read(struct file *file, char __user * buf, size_t lbuf, loff_t * ppos)
{
    struct asp_mycdev *dev = file->private_data;
	int nbytes;

    if (down_interruptible(&dev->sem)) return -ERESTARTSYS;

	if ((lbuf + *ppos) > dev->ramsize) {
		pr_info("trying to read past end of device,"
			"aborting because this is just a stub!\n");
        up(&dev->sem);
		return 0;
	}


	nbytes = lbuf - copy_to_user(buf, dev->ramdisk + *ppos, lbuf);
	*ppos += nbytes;

    up(&dev->sem);

	pr_info("\n READING function, nbytes=%d, pos=%d lbuf=%zu\n", nbytes, (int)*ppos, lbuf);
	return nbytes;
}

static ssize_t my_write(struct file *file, const char __user * buf, size_t lbuf, loff_t * ppos)
{
    struct asp_mycdev *dev = file->private_data;
	int nbytes;

    if (down_interruptible(&dev->sem)) return -ERESTARTSYS;

	if ((lbuf + *ppos) > dev->ramsize) {
		pr_info("trying to read past end of device,"
			"aborting because this is just a stub!\n");
        up(&dev->sem);
		return 0;
	}
	nbytes = lbuf - copy_from_user(dev->ramdisk + *ppos, buf, lbuf);
	*ppos += nbytes;

    up(&dev->sem);

	pr_info("\n WRITING function, nbytes=%d, pos=%d\n", nbytes, (int)*ppos);
	return nbytes;
}

module_init(my_init);
module_exit(my_exit);

MODULE_AUTHOR("user");
MODULE_LICENSE("GPL v2");
