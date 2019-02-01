ifeq ($(KERNELRELEASE),)


KERNELDIR = ~/sources/rpi-4.9/
PWD := $(shell pwd)

default:
	$(MAKE) ARCH=arm CROSS_COMPILE=arm-poky-linux-gnueabi- -C $(KERNELDIR) M=$(PWD) modules
	
else
	obj-m:= hello.o
	ccflags-y := -std=gnu99 -Wno-declaration-after-statement
endif

