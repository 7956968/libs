
export ARCH=arm
export CROSS_COMPILE=arm-himix200-linux-

LINUX_ROOT = /home/zucker/Project/1.hisi/1.sdk/4.Hi3516CV500_SDK_V2.0.1.0/osdrv/opensource/kernel/linux-4.9.y

obj-m := gpioconfig.o

EXTRA_CFLAGS+=-I$(PWD)/..


default:
	make -C $(LINUX_ROOT) M=$(PWD) modules 
clean: 
	make -C $(LINUX_ROOT) M=$(PWD) clean 



