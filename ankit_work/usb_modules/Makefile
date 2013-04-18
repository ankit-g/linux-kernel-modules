	obj-m := zero.o


	KERNELDIR ?= /lib/modules/$(shell uname -r)/build
	PWD := $(shell pwd)
default:
	$(MAKE)  -C $(KERNELDIR) M=$(PWD) modules
	@rm -rf *.o  *.mod.c .*.cmd ${INSTALL_DIR}
