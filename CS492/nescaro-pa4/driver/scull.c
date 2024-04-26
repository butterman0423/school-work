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

#include <asm/current.h> 	/* current ptr */
#include <linux/list.h>		/* LL struct + macros */
#include <linux/mutex.h>	/* Mutexes */

#include "scull.h"		/* local definitions */

/* Struct for LL */
struct task_node {
	pid_t pid;
	pid_t tgid;
	struct list_head lst;
};

/*
 * Our parameters which can be set at load time.
 */

static int scull_major =   SCULL_MAJOR;
static int scull_minor =   0;
static int scull_quantum = SCULL_QUANTUM;

static LIST_HEAD(ts_head); 			/* Initialize LL */
static DEFINE_MUTEX(ll_lock);		/* Initialize Mutex */

module_param(scull_major, int, S_IRUGO);
module_param(scull_minor, int, S_IRUGO);
module_param(scull_quantum, int, S_IRUGO);

MODULE_AUTHOR("nescaro");
MODULE_LICENSE("Dual BSD/GPL");

static struct cdev scull_cdev;		/* Char device structure */

static void set_task_info(struct task_info* dest) {
	dest->__state = current->__state;
	dest->cpu = current->on_cpu;
	dest->prio = current->prio;
	dest->pid = current->pid;
	dest->tgid = current->tgid;
	dest->nvcsw = current->nvcsw;
	dest->nivcsw = current->nivcsw;
}

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

/*
 * The ioctl() implementation
 */

static long scull_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	int err = 0, tmp;
	int retval = 0;
	struct task_info t_info;
    struct task_node* t_node;
	struct task_node* tmp_node;

	/*
	 * extract the type and number bitfields, and don't decode
	 * wrong cmds: return ENOTTY (inappropriate ioctl) before access_ok()
	 */
	if (_IOC_TYPE(cmd) != SCULL_IOC_MAGIC) return -ENOTTY;
	if (_IOC_NR(cmd) > SCULL_IOC_MAXNR) return -ENOTTY;

	err = !access_ok((void __user *)arg, _IOC_SIZE(cmd));
	if (err) return -EFAULT;

	switch(cmd) {

	case SCULL_IOCRESET:
		scull_quantum = SCULL_QUANTUM;
		break;
        
	case SCULL_IOCSQUANTUM: /* Set: arg points to the value */
		retval = __get_user(scull_quantum, (int __user *)arg);
		break;

	case SCULL_IOCTQUANTUM: /* Tell: arg is the value */
		scull_quantum = arg;
		break;

	case SCULL_IOCGQUANTUM: /* Get: arg is pointer to result */
		retval = __put_user(scull_quantum, (int __user *)arg);
		break;

	case SCULL_IOCQQUANTUM: /* Query: return it (it's positive) */
		return scull_quantum;

	case SCULL_IOCXQUANTUM: /* eXchange: use arg as pointer */
		tmp = scull_quantum;
		retval = __get_user(scull_quantum, (int __user *)arg);
		if (retval == 0)
			retval = __put_user(tmp, (int __user *)arg);
		break;

	case SCULL_IOCHQUANTUM: /* sHift: like Tell + Query */
		tmp = scull_quantum;
		scull_quantum = arg;
		return tmp;

	case SCULL_IOCIQUANTUM:
		set_task_info(&t_info);
		retval = copy_to_user(\
					(struct task_info __user*) arg, \
					&t_info, \
					sizeof(struct task_info) );

		// Copy to LL
		t_node = kmalloc(sizeof(struct task_node), GFP_KERNEL);
		if(!t_node) {
			printk( KERN_ERR "Error: kmalloc() failed\n" );
			break;
		}
		
		t_node->pid = t_info.pid;
		t_node->tgid = t_info.tgid;
		INIT_LIST_HEAD( &t_node->lst );
		
		tmp = 0;	// Push lock

		mutex_lock(&ll_lock);
		
		/** Critical Section **/
		
		// Check if exists
		list_for_each_entry(tmp_node, &ts_head, lst) 
		{
			if( tmp_node->pid == t_node->pid && tmp_node->tgid == t_node->tgid ) {
				kfree(t_node);
				tmp = 1;
				break;
			}
		}
		
		if(!tmp) {
			list_add(&t_node->lst, &ts_head);
		}

		/** END Critical Section **/

		mutex_unlock(&ll_lock);
		
		break;

	default:  /* redundant, as cmd was checked against MAXNR */
		return -ENOTTY;
	}
	return retval;
}

struct file_operations scull_fops = {
	.owner =    THIS_MODULE,
	.unlocked_ioctl = scull_ioctl,
	.open =     scull_open,
	.release =  scull_release,
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
	struct task_node* t_node;
	struct task_node* tmp;
	int idx = 1;
	
	dev_t devno = MKDEV(scull_major, scull_minor);

	/* Get rid of the char dev entry */
	cdev_del(&scull_cdev);

	/* cleanup_module is never called if registering failed */
	unregister_chrdev_region(devno, 1);

	/* Print out (and free) all items in LL */
	
	list_for_each_entry_safe(t_node, tmp, &ts_head, lst) 
	{
		printk( KERN_INFO "Task %d: PID %d, TGID %d\n", \
				idx++, t_node->pid, t_node->tgid );
		list_del(&t_node->lst);
		kfree(t_node); // Might cause issues
	}
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

	return 0; /* succeed */

  fail:
	scull_cleanup_module();
	return result;
}

module_init(scull_init_module);
module_exit(scull_cleanup_module);
