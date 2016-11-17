#include <linux/fs.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/module.h>
#include <asm/uaccess.h>
#include <linux/cdev.h>
#include <linux/errno.h>
#include <linux/device.h>

#include <linux/version.h>
#include <linux/ioport.h>
#include <asm/io.h>
#include <linux/kdev_t.h>
#include <linux/types.h>

#include "ioctl_test.h"

#define DEVICENAME "LED"
#define TEXTLENGTH 100

#define IOMUXR 0x20E0000
#define GPIO_DR 0x20A4000

#define LED_OFF 0x00000000
#define LED_ON 0x02000000
#define MEM_REG 0x02000000
#define MEM_FREE 0x00000000
#define MEM_REGION 0x1FF

char msg[TEXTLENGTH];
int led;

int hello_open(struct inode *inode, struct file *filp);
int hello_release(struct inode *inode, struct file *filp);
//int hello_read(struct file * file, char * buf, size_t count, loff_t *ppos);
//int hello_write(struct file * file, char * buf, size_t count, loff_t *ppos);
static long hello_ioctl(struct file *file, unsigned int cmd, unsigned long arg);

int result;
int major;
int minor = 0;
int numofdevice = 1;
dev_t dev;
struct cdev cdev;

static int iomux_register=0,led_pin_register=0;

static const struct file_operations hello_fops = {
	.owner 	= THIS_MODULE,
	.open	= hello_open,
	.release= hello_release,
	.unlocked_ioctl = hello_ioctl,
};

static long hello_ioctl(struct file *file, unsigned int cmd, unsigned long arg){
	int retval;
	
	switch (cmd){
		case IOCTL_LEDOFF:
			iowrite32(LED_OFF,(led_pin_register+0x0));
		break;

		case IOCTL_LEDON:
			iowrite32(LED_ON,(led_pin_register+0x0));
		break;

		case IOCTL_READ:
			//iowrite32(LED_OFF,(led_pin_register+0x0));
			if(copy_to_user((void *)arg, &msg, 100)){
				return -EACCES;
			}	
		break;

		case IOCTL_WRITE:
			//iowrite32(LED_ON,(led_pin_register+0x0));
			if(copy_from_user(&msg, (void *)arg, 100)){
				return -EACCES;
			}
		

		break;
		default:
			return -EINVAL;
	}
	return 0;
}

static int hello_init(void){
	result = alloc_chrdev_region(&dev,minor,numofdevice,DEVICENAME); 
	if(result < 0){
		printk(KERN_ALERT "HelloWorld did not registered!\n");
		return result;
	}
	major = MAJOR(dev);
	printk(KERN_ALERT "HelloWorld registerd to major : %d\n",major);
	cdev_init(&cdev, &hello_fops);
	result = cdev_add(&cdev, dev, numofdevice);
	if(result < 0){
		printk(KERN_ALERT "HelloWorld failed to add cdev\n");
		return result;
	}

	/*LED INIT blink here*/

	/* This will request the memory region 
	 * If this return a NULL, the memory region has allready used
	 * This happence by the kernel when compiling
	 * But still we can use the memory region(This is not the good practice)
	 * You should check the memory rigion before using it!!!
	 * Return -EBUSY if memory region allready allocated!
	 */
	
	request_mem_region(IOMUXR, MEM_REGION,"GPIO");
    	request_mem_region(GPIO_DR, MEM_REGION,"GPIO2");
 
    	iomux_register = ioremap(IOMUXR, MEM_REGION);
    	led_pin_register = ioremap(GPIO_DR, MEM_REGION);

	/* IOMUX */
   	iowrite32(0x00000015,(iomux_register+0xB8)); 
        
    	/* DIRECTION */
    	iowrite32(MEM_REG,(led_pin_register+0x4)); 

    	/* LED OFF */ 
	iowrite32(LED_OFF,(led_pin_register+0x0));	


	printk(KERN_ALERT "HelloWorld init done\n");
	return 0;
}

static void hello_exit(void){
	cdev_del(&cdev);
	unregister_chrdev_region(dev, numofdevice);

	/*LED EXIT OFF blink here*/

    	iowrite32(MEM_FREE,(iomux_register+0xB8));
    	iowrite32(LED_OFF,(led_pin_register+0x0));
    	iowrite32(MEM_FREE,(led_pin_register+0x4));

    	iounmap(IOMUXR);
    	iounmap(GPIO_DR);

    	release_mem_region(IOMUXR,MEM_REGION);
    	release_mem_region(GPIO_DR,MEM_REGION);
    
	printk(KERN_ALERT "HelloWorld exit called\n");
}

int hello_open(struct inode *inode, struct file *filp){

	/*LED OPEN blink here*/
        printk(KERN_ALERT "HelloWorld open Called\n");
        return 0;
}

int hello_release(struct inode *inode, struct file *filp){
	/*
	LED RELEASE blink here
	*/
	printk(KERN_ALERT "HelloWorld release called\n");
	return 0;
}
/*
int hello_read(struct file * file, char * buf, size_t count, loff_t *ppos){
	printk(KERN_ALERT "HelloWorld read called\n");
	if(hello_str != NULL && count != 0 && count < TEXTLENGTH ){
        	if(copy_to_user(buf, hello_str, count))
                	return -EINVAL;	
		*ppos = count;
	/*
	LED READ blink here
	* /
        	return count;
	}
}


int hello_write(struct file * file, char * buf, size_t count, loff_t *ppos){
	printk(KERN_ALERT "HelloWorld write called\n");
	
	if(hello_str == NULL && buf != NULL && count != 0 && count < TEXTLENGTH){
	        if(copy_from_user(hello_str,buf, count))
                	return -EINVAL;
        	*ppos = count;
	/*
	LED WRITE blink here
	* /
	 	return count;
	}
       
}
*/

module_init(hello_init);
module_exit(hello_exit);
