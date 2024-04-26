/**
 * Name: Nathaniel Escaro
 * Pledge: I pledge my honor that I have abided by the Stevens Honor System.
 */

#include <linux/syscalls.h>

SYSCALL_DEFINE1(nescaro_syscall, char __user *, str) {	
	long len;
	int numRep;
	char kstr[32];
	
	if(str == NULL) {
		return -1;
	}

	len = strnlen_user(str, 32);
	if(len == 0 || len > 32) {
		return -1;
	}

	if(strncpy_from_user(kstr, str, 32) == -EFAULT) {
		return -EFAULT;
	}

	printk(KERN_INFO "before: %s\n", kstr);

	numRep = 0;	
	for(int i = 0; i < len; i++) {
		switch(kstr[i]) {
			case 'a':
			case 'e':
			case 'i':
			case 'o':
			case 'u':
			case 'y':
				kstr[i] = 'N';
				numRep++;
		}
	}

	printk(KERN_INFO "after: %s\n", kstr);
	
	if( copy_to_user(str, kstr, len + 1) ) {
		return -EFAULT;
	}

	return numRep;
}
