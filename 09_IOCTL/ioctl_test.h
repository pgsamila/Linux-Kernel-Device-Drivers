#ifndef IOCTL_TEST_H
#define IOCTL_TEST_H
#include <linux/ioctl.h>

#define IOC_MAGIC 'k'

#define IOCTL_LEDOFF _IOW(IOC_MAGIC,1, void *)
#define IOCTL_LEDON _IOW(IOC_MAGIC,2, void *)
#define IOCTL_WRITE _IOW(IOC_MAGIC,3, void *)
#define IOCTL_READ _IOR(IOC_MAGIC,4,void*)

#endif
