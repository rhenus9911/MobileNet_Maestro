#!/bin/bash

# 모듈 소스 파일
MODULE_NAME="nobrand"
SRC_FILE="${MODULE_NAME}.c"
KDIR := /usr/src/linux

# 커널 모듈 빌드용 Makefile 생성
cat <<EOF > Makefile
obj-m += testkernel.o
KDIR := /usr/src/linux
PWD := $(shell pwd)

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
EOF

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
