#include <linux/module.h>
#include <linux/kernel.h>

int init_module(void){
	printk(KERN_INFO "hello wORLD!\n");
	return 0;
}

void cleanup_module(void){
	printk(KERN_INFO "Good Bye World1\n");
}

