.PHONY: build clean test reset

obj-m += doggo_char.o
BUILD_DIR:=/lib/modules/$(shell uname -r)/build
CWD:=$(shell pwd)
LOCAL_BUILD_DIR:=$(CWD)/build

build: clean
	mkdir -p $(LOCAL_BUILD_DIR)
	cp *.c $(LOCAL_BUILD_DIR)/
	$(MAKE) -C $(BUILD_DIR) M=$(CWD) modules
	mv *.ko $(LOCAL_BUILD_DIR)/
	$(MAKE) clean-cmd

clean:
	rm -rf $(LOCAL_BUILD_DIR)
	$(MAKE) -C $(BUILD_DIR) M=$(CWD) clean
clean-cmd:
	rm -f *.ko *.mod.c *.mod.o *.o *.mod .*.cmd modules.order Module.symvers

run: build
	cd build/ && sudo insmod doggo_char.ko
	sudo mknod /dev/doggo_char c 240 0
	sudo chmod 666 /dev/doggo_char
load:
	cd build/ && sudo insmod doggo_char.ko
	sudo mknod /dev/doggo_char c 240 0
	sudo chmod 666 /dev/doggo_char
test:
	echo "I write to char file1" >> /dev/doggo_char
	cat /dev/doggo_char
reset:
	sudo rm /dev/doggo_char
	sudo rmmod -f doggo_char
all: run test

