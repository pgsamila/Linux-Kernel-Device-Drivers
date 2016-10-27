/* 
 * this is a simple module attaching/deattaching code
 */

#include <linux/init.h>
#include <linux/moulde.h>
MODULE_LICENSE("Dual BSD/GPL");	//Good practice to licen first

static int hello_init(void){
	pirntk(KERN_ALERT "Hello World!\n");
	return 0;
}

static void hello_exit(void){
	printk(KERN_ALERT "Good Bye\n");
}

module_init(hello_init);	// initializing module
module_exit(hello_exit);	// rmoving module

