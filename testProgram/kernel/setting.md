### 라즈베리파이4 커널 세팅

1. 종속성 설치

```bash
$	sudo apt install git bc bison flex libssl-dev make
```

2. 커널 소스코드 다운

```bash
cd /usr/src
sudo git clone --depth=1 https://github.com/raspberrypi/linux
```

3. Config 파일 생성

```bash
cd linux
KERNEL=kernel8 # raspberry pi4 64bit settting
sudo make bcm2711_defconfig
```

4. Build

```bash
sudo make -j4 Image.gz modules dtbs # core가 4개이므로 j4, 64bit이므로 Image.gz
sudo make modules_install
sudo cp arch/arm64/boot/dts/broadcom/*.dtb /boot/firmware
sudo cp arch/arm64/boot/dts/overlays/*.dtb* /boot/firmware/overlays/
sudo cp arch/arm64/boot/dts/overlays/README /boot/firmware/overlays/
sudo cp arch/arm64/boot/Image.gz /boot/firmware/$KERNEL.img
```

5. kernel version

```bash
uname -r
```

6. reboot

```bash
sudo reboot 
uname -r
```


7. testkernel.c
```c
#include <linux/module.h>
#include <linux/kernel.h>

int __init test_init(void) {
    // 초기화 코드
    printk(KERN_INFO "Test module initialized\n");
    return 0;
}

void __exit test_exit(void) {
    // 종료 코드
    printk(KERN_INFO "Test module exited\n");
}

module_init(test_init);
module_exit(test_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("A simple test module");

```

8.Makefile
```Makefile
obj-m += testkernel.o
KDIR := /usr/src/linux
PWD := $(shell pwd)

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

```

9.testkernel.sh   
```shell
#!/bin/bash

# 모듈 소스 파일
MODULE_NAME="testkernel"
SRC_FILE="${MODULE_NAME}.c"

# 커널 모듈 빌드용 Makefile 생성
# cat <<EOF > Makefile
# obj-m += ${MODULE_NAME}.o

# all:
#	\$(MAKE) -C /lib/modules/\$(uname -r)/build M=\$(pwd) modules

# clean:
#	\$(MAKE) -C /lib/modules/\$(uname -r)/build M=\$(pwd) clean
# EOF

# 커널 모듈 빌드
make

# 빌드 성공 여부 확인
if [ $? -ne 0 ]; then
    echo "Kernel module build failed."
    exit 1
fi

# 커널 모듈 로드
sudo insmod ${MODULE_NAME}.ko

# 모듈이 로드되었는지 확인
if lsmod | grep -q ${MODULE_NAME}; then
    echo "Kernel module ${MODULE_NAME} loaded successfully."
else
    echo "Failed to load kernel module ${MODULE_NAME}."
    exit 1
fi

# 커널 로그 확인
dmesg | tail -n 10

# 5초 대기 후 모듈 언로드
sleep 5
sudo rmmod ${MODULE_NAME}

# 언로드 확인
if lsmod | grep -q ${MODULE_NAME}; then
    echo "Failed to unload kernel module ${MODULE_NAME}."
else
    echo "Kernel module ${MODULE_NAME} unloaded successfully."
fi

# 커널 로그 확인
dmesg | tail -n 10

```

10.shell code 실행
'''bash
./testkernel.sh
'''
