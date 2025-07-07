.PHONY: build clean

obj-m += doggo_basic.o
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
