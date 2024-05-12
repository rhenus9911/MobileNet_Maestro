KERNEL_SRC = /usr/src/linux
TARGET := app
obj-m := nobrand.o

all: driver app

driver:
	make -C $(KERNEL_SRC) M=$(PWD) modules

app:
	$(CC) -o $@ $@.c

clean:
	make -C $(KERNEL_SRC) M=$(PWD) clean
	rm -f *.o $(TARGET)
