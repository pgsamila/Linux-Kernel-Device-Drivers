# obj-m is a list of object files (The .o file)

obj-m := hello.o

# KDIR is the location of kerlen source.

KDIR := /lib/modules/$(shell uname -r)/build

# PWD is the current working derectory and location of the modules

PWD := $(shell pwd)

# default is the default make target
# source and compile only the modules in the PWD directory

default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
