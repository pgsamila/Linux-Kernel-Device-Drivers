#include <linux/kernel.h>
#include <linux/module.h>

int init_module(void){
	printk(KERN_INFO "hello World\n");
	return 0;
}
