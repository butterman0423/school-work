/*
 * scull.c -- the bare scull char module
 *
 * Copyright (C) 2001 Alessandro Rubini and Jonathan Corbet
 * Copyright (C) 2001 O'Reilly & Associates
 *
 * The source code in this file can be freely used, adapted,
 * and redistributed in source or binary form, so long as an
 * acknowledgment appears in derived source files.  The citation
 * should list that the code comes from the book "Linux Device
 * Drivers" by Alessandro Rubini and Jonathan Corbet, published
 * by O'Reilly & Associates.   No warranty is attached;
 * we cannot take responsibility for errors or fitness for use.
 *
 */

/**
 * Name: Nathaniel Escaro
 * Pledge: I pledge my honor that I have abided by the Stevens Honor System
 */

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>

#include <linux/kernel.h>	/* printk() */
#include <linux/slab.h>		/* kmalloc() */
#include <linux/fs.h>		/* everything... */
#include <linux/errno.h>	/* error codes */
#include <linux/types.h>	/* size_t */
#include <linux/cdev.h>

#include <linux/uaccess.h>	/* copy_*_user */
#include <linux/mutex.h>
#include <linux/semaphore.h>

#include "scull.h"		/* local definitions */

/*
 * Our parameters which can be set at load time.
 */

static int scull_major =   SCULL_MAJOR;
static int scull_minor =   0;
static int scull_fifo_elemsz = SCULL_FIFO_ELEMSZ_DEFAULT; /* ELEMSZ */
static int scull_fifo_size   = SCULL_FIFO_SIZE_DEFAULT;   /* N      */

module_param(scull_major, int, S_IRUGO);
module_param(scull_minor, int, S_IRUGO);
module_param(scull_fifo_size, int, S_IRUGO);
module_param(scull_fifo_elemsz, int, S_IRUGO);

MODULE_AUTHOR("nescaro");
MODULE_LICENSE("Dual BSD/GPL");

static struct cdev scull_cdev;		/* Char device structure */

static char* fifo = NULL;					/* FIFO queue */
static char* fifo_start;
static char* fifo_end;
static struct semaphore fifo_sem_empty, fifo_sem_full;
static DEFINE_MUTEX(fifo_mutex);

#define fifo_size 		scull_fifo_size * (scull_fifo_elemsz + sizeof(size_t))

#define STL(sem)		if( down_interruptible(sem) == -EINTR || \
							mutex_lock_interruptible(&fifo_mutex) == -EINTR ) \
						{ printk(KERN_ERR "Process interrupted!"); return -EINTR; }

/*
 * Open and close
 */

static int scull_open(struct inode *inode, struct file *filp)
{
	printk(KERN_INFO "scull open\n");
	return 0;          /* success */
}

static int scull_release(struct inode *inode, struct file *filp)
{
	printk(KERN_INFO "scull close\n");
	return 0;
}

/**
 * Pointer wrapping
 */

static void fifo_next(char** p) {
	*p += sizeof(size_t) + scull_fifo_elemsz;

	if(*p >= fifo + fifo_size) {
		*p = fifo + (*p - (fifo + fifo_size));
	}
}

/*
 * Read and Write
 */
static ssize_t scull_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
	/* filp and f_pos are not used */
	
	size_t len;

	STL(&fifo_sem_full);

	// CRIT SECTION //
	
	len = *(size_t*) fifo_start;
	if(count > len) {
		count = len;
	}

	if( copy_to_user(buf, fifo_start + sizeof(size_t), count) ) {
		printk(KERN_ERR "Error: Failed to write message to user.");
		
		mutex_unlock(&fifo_mutex);
		up(&fifo_sem_empty);
		return -EFAULT;
	}
	fifo_next(&fifo_start);

	// END SECTION //

	mutex_unlock(&fifo_mutex);
	up(&fifo_sem_empty);
	
	printk(KERN_INFO "scull read\n");
	return count;
}


static ssize_t scull_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
	/* filp and f_pos are not used */
	
	char* tmp;

	if(count > scull_fifo_elemsz) {
		count = (size_t) scull_fifo_elemsz;
	}
	
	STL(&fifo_sem_empty);

	// CRIT SECTION //
	
	// Write message first
	tmp = fifo_end + sizeof(size_t);
	if( copy_from_user(tmp, buf, count) ) {
		printk(KERN_ERR "Error: Failed to read message from user.");
		
		mutex_unlock(&fifo_mutex);
		up(&fifo_sem_full);
		return -EFAULT;
	}

	*(size_t*) fifo_end = count;
	fifo_next(&fifo_end);

	// END SECTION //
	
	mutex_unlock(&fifo_mutex);
	up(&fifo_sem_full);
	
	printk(KERN_INFO "scull write\n");
	return 0;
}

/*
 * The ioctl() implementation
 */
static long scull_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{

	int err = 0;
	int retval = 0;
    
	/*
	 * extract the type and number bitfields, and don't decode
	 * wrong cmds: return ENOTTY (inappropriate ioctl) before access_ok()
	 */
	if (_IOC_TYPE(cmd) != SCULL_IOC_MAGIC) return -ENOTTY;
	if (_IOC_NR(cmd) > SCULL_IOC_MAXNR) return -ENOTTY;

	err = !access_ok((void __user *)arg, _IOC_SIZE(cmd));
	if (err) return -EFAULT;

	switch(cmd) {
	case SCULL_IOCGETELEMSZ:
		return scull_fifo_elemsz;

	default:  /* redundant, as cmd was checked against MAXNR */
		return -ENOTTY;
	}
	return retval;

}

struct file_operations scull_fops = {
	.owner 		= THIS_MODULE,
	.unlocked_ioctl = scull_ioctl,
	.open 		= scull_open,
	.release	= scull_release,
	.read 		= scull_read,
	.write 		= scull_write,
};

/*
 * Finally, the module stuff
 */

/*
 * The cleanup function is used to handle initialization failures as well.
 * Thefore, it must be careful to work correctly even if some of the items
 * have not been initialized
 */
void scull_cleanup_module(void)
{
	dev_t devno = MKDEV(scull_major, scull_minor);

	if(fifo) {
		kfree(fifo);
	}

	/* Get rid of the char dev entry */
	cdev_del(&scull_cdev);

	/* cleanup_module is never called if registering failed */
	unregister_chrdev_region(devno, 1);
}

int scull_init_module(void)
{
	int result;
	dev_t dev = 0;

	/*
	 * Get a range of minor numbers to work with, asking for a dynamic
	 * major unless directed otherwise at load time.
	 */
	if (scull_major) {
		dev = MKDEV(scull_major, scull_minor);
		result = register_chrdev_region(dev, 1, "scull");
	} else {
		result = alloc_chrdev_region(&dev, scull_minor, 1, "scull");
		scull_major = MAJOR(dev);
	}
	if (result < 0) {
		printk(KERN_WARNING "scull: can't get major %d\n", scull_major);
		return result;
	}

	cdev_init(&scull_cdev, &scull_fops);
	scull_cdev.owner = THIS_MODULE;
	result = cdev_add (&scull_cdev, dev, 1);
	/* Fail gracefully if need be */
	if (result) {
		printk(KERN_NOTICE "Error %d adding scull character device", result);
		goto fail;
	}
	
	fifo = (char*) kmalloc(fifo_size, GFP_KERNEL);
	if(!fifo) {
		printk(KERN_ERR "FIFO allocation failed\n");
		goto fail;
	}

	fifo_start = fifo;
	fifo_end = fifo;
	sema_init(&fifo_sem_empty, scull_fifo_size);
	sema_init(&fifo_sem_full, 0);

	printk(KERN_INFO "scull: FIFO SIZE=%u, ELEMSZ=%u\n", scull_fifo_size, scull_fifo_elemsz);

	return 0; /* succeed */

  fail:
	scull_cleanup_module();
	return result;
}

module_init(scull_init_module);
module_exit(scull_cleanup_module);
