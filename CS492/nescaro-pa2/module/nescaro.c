/**
 * Name: Nathaniel Escaro
 * Pledge: I pledge my honor that I have abided by the Stevens Honor System
 */

#include <linux/module.h>
#include <linux/init.h>
#include <asm/current.h>

static int init(void) {
	printk(KERN_INFO "Hello World from Nathaniel Escaro (nescaro)\n");
	return 0;
}

static void destroy(void) {
	printk(KERN_INFO "PID is %i and program name is %s\n",
			current->pid, current->comm);
}

module_init(init);
module_exit(destroy);
MODULE_LICENSE("Dual BSD/GPL");
