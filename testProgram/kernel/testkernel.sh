#!/bin/bash

# 모듈 소스 파일
MODULE_NAME="testkernel"
SRC_FILE="${MODULE_NAME}.c"

# 커널 모듈 빌드용 Makefile 생성
cat <<EOF > Makefile
obj-m += ${MODULE_NAME}.o

all:
\tmake -C /lib/modules/\$(uname -r)/build M=\$(pwd) modules

clean:
\tmake -C /lib/modules/\$(uname -r)/build M=\$(pwd) clean
EOF

# 커널 모듈 빌드
make

# 커널 모듈 로드
sudo insmod ${MODULE_NAME}.ko

# 모듈이 로드되었는지 확인
lsmod | grep ${MODULE_NAME}

# 커널 로그 확인
dmesg | tail -n 10

# 5초 대기 후 모듈 언로드
sleep 5
sudo rmmod ${MODULE_NAME}

# 언로드 확인
dmesg | tail -n 10
