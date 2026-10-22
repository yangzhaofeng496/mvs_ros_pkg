
CONFIG_MODULE_SIG=n

PWD = `pwd`
KERNEL_FOLDER := /lib/modules/$(shell uname -r)/build

PLATFORM := $(shell uname -m)

# 获取内核版本号并转换为可比较的格式
KERNEL_VERSION := $(shell uname -r | awk -F'.' '{printf "%d%03d%03d", $$1, $$2, $$3}')
KERNEL_6_15 := 6015000

ifeq ($(PLATFORM), x86_64)
# 内核版本 >= 6.15.0 使用 kernel6_15 目录，否则使用 gcc4_8 目录
ifneq ($(shell expr $(KERNEL_VERSION) \>= $(KERNEL_6_15)), 0)
LIB_DIR = kernel6_15
else
LIB_DIR = gcc4_8
endif
endif

EXTRA_CFLAGS += -D_UNIX_ -D_PT_KERNEL_ -Wno-error=implicit-function-declaration

INCLUDE_DIR = -I$(src)/./inc
ccflags-y := $(INCLUDE_DIR)

obj-m += mvfgvirtualserial.o 
mvfgvirtualserial-y := 			\
	mv_atomic.o 		\
	mv_device.o			\
	mv_event.o 			\
	mv_kthread.o 		\
	mv_lock.o 			\
	mv_memory.o 		\
	mv_memorymap.o 		\
	mv_misc.o 			\
	mv_time.o 			\
	mv_io.o 			\
	mv_workqueue.o 		\
	mv_module.o 		\
	mv_dma.o			\
	mv_socket.o			\
	mv_platform.o		\
    drvmodlib.o

all:
ifeq ($(PLATFORM), x86_64)
	ar -x $(PLATFORM)/$(LIB_DIR)/libmvfgvirtualserial.a
else ifeq ($(PLATFORM), i686)
	ar -x i386/libmvfgvirtualserial.a
else
	ar -x $(PLATFORM)/libmvfgvirtualserial.a
endif
	@make -C $(KERNEL_FOLDER) M=$(PWD) modules

clean:
	@rm -rf *.o *.o.* .??* mvfgvirtualserial.mod.c Module.markers Module.symvers modules.order *.ko
install:
	/sbin/insmod mvfgvirtualserial.ko
remove:
	/sbin/rmmod mvfgvirtualserial
