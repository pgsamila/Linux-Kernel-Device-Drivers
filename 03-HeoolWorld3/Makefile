obj-m := hello-1.o
obj-m := hello-2.o
obj-m := hello-3.o

KDIR := /lib/modules/$(shell uname -r)/build

PWD := $(shell pwd)

default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

