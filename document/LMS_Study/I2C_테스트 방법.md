## 1. 연결의 정상성 테스트
라즈베리 파이에서 I2C 활성화
sudo raspi-config를 실행하고 "Interfacing Options"에서 "I2C"를 활성화

I2C 도구 설치
i2c-tools와 libi2c-dev 라이브러리 설치

```
sudo apt-get update
sudo apt-get install i2c-tools libi2c-dev
```

```
#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

// 장치의 I2C 주소, 예: 0x48
#define I2C_ADDR 0x48

int main() {
    int file;
    char *bus = "/dev/i2c-1"; // I2C 버스 파일, 라즈베리 파이에서는 대개 /dev/i2c-1
    if ((file = open(bus, O_RDWR)) < 0) {
        perror("Failed to open the bus.");
        return 1;
    }
    
    // I2C 슬레이브 장치로의 접근을 가능하게 함
    if (ioctl(file, I2C_SLAVE, I2C_ADDR) < 0) {
        perror("Failed to connect to the sensor.\n");
        return 1;
    }

    // 스캔을 위한 데이터를 읽기 시도
    char data[2] = {0};
    if (read(file, data, 2) != 2) {
        perror("Failed to read from the I2C device.\n");
        return 1;
    } else {
        printf("Data read: %d %d\n", data[0], data[1]);
    }

    close(file);
    return 0;
}
```

##2. 통신의 정상성 테스트

주소가 0x48인 장치의 예제

```
#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>

int main() {
    int fd;  // 파일 디스크립터
    int data;  // 읽은 데이터를 저장할 변수

    // I2C 장치 초기화 (장치의 주소는 예를 들어 0x48)
    fd = wiringPiI2CSetup(0x48);
    if (fd == -1) {
        fprintf(stderr, "Failed to init I2C communication.\n");
        return 1;
    }
    printf("I2C device initialized successfully.\n");

    // I2C 장치로부터 데이터 읽기
    data = wiringPiI2CRead(fd);
    if (data == -1) {
        fprintf(stderr, "Failed to read from the I2C device.\n");
        return 1;
    }
    printf("Read data: %d\n", data);

    return 0;
}
```
