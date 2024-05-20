obj-m += testkernel.o

all:
\tmake -C /lib/modules/$(uname -r)/build M=$(pwd) modules

clean:
\tmake -C /lib/modules/$(uname -r)/build M=$(pwd) clean
