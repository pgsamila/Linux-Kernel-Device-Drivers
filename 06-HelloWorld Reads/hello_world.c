#include <linux/fs.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/module.h>
#include <asm/uaccess.h>
#include <linux/cdev.h>
#define DEVICENAME "hello_world"
#define READINGDATA "Hello World Reads!\n"

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
	printk(KERN_ALERT "HelloWorld is Reading...\n");
        char *hello_str = READINGDATA;
        int len = strlen(hello_str);
        if(count < len)
                return -EINVAL;
        if(*ppos != 0)
                return 0;
        if(copy_to_user(buf, hello_str, len))
                return -EINVAL;
        *ppos = len;
        return len;
}


int hello_write(struct file * file, char * buf, size_t count, loff_t *ppos){
	printk(KERN_ALERT "HelloWorld is Writing...\n");

	struct 

}


module_init(hello_init);
module_exit(hello_exit);
