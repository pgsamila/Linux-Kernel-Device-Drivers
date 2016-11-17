#include <linux/fs.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/module.h>
#include <asm/uaccess.h>
#include <linux/cdev.h>
#include <linux/errno.h>

#define DEVICENAME "hello_world"
#define READINGDATA "Hello World Reads!\n"
#define TEXTLENGTH 100

char *hello_str[TEXTLENGTH];

int hello_open(struct inode *inode, struct file *filp);
int hello_release(struct inode *inode, struct file *filp);
int hello_read(struct file * file, char * buf, size_t count, loff_t *ppos);
int hello_write(struct file * file, char * buf, size_t count, loff_t *ppos);

int result;
int major;
int minor = 0;
int numofdevice = 1;
dev_t dev;
struct cdev cdev;

static const struct file_operations hello_fops = {
	.owner 	= THIS_MODULE,
	.open	= hello_open,
	.release= hello_release,
	.read	= hello_read,
	.write	= hello_write,
};

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
//		printk(KERN_ERR "Unable to register \"Hello,World!\" misc device\n");
	printk(KERN_ALERT "HelloWorld init done\n");
	return 0;
}

static void hello_exit(void){
	cdev_del(&cdev);
	unregister_chrdev_region(dev, numofdevice);
	printk(KERN_ALERT "HelloWorld exit called\n");
}

int hello_open(struct inode *inode, struct file *filp){
        printk(KERN_ALERT "HelloWorld open Called\n");
        return 0;
}

int hello_release(struct inode *inode, struct file *filp){
	printk(KERN_ALERT "HelloWorld release called\n");
	return 0;
}

int hello_read(struct file * file, char * buf, size_t count, loff_t *ppos){
	printk(KERN_ALERT "HelloWorld read called\n");
	if(hello_str != NULL && count != 0 && count < TEXTLENGTH ){
        	if(copy_to_user(buf, hello_str, count))
                	return -EINVAL;	
		*ppos = count;
        	return count;
	}
}


int hello_write(struct file * file, char * buf, size_t count, loff_t *ppos){
	printk(KERN_ALERT "HelloWorld write called\n");
	
	if(hello_str == NULL && buf != NULL && count != 0 && count < TEXTLENGTH){
	        if(copy_from_user(hello_str,buf, count))
                	return -EINVAL;
        	*ppos = count;
	 	return count;
	}
       
}


module_init(hello_init);
module_exit(hello_exit);
