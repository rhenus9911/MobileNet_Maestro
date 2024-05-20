KERNEL_SRC=/lib/modules/$(shell uname -r)/build
obj-m := testkernel.o

go:
    make -C $(KERNEL_SRC) M=$(PWD) modules