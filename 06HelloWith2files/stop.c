#include <linux/kernel.h>
#include <linux/module.h>

void cleanup_module(){
	printk(KERN_INFO "Short is the life of a kenel\n");
}

