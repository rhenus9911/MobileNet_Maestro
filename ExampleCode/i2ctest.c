#include <wiringPiI2C.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int fd;
    int deviceAddress = 0x27; // I2C 장치 주소
    //int bus = 1; // I2C 버스 번호

    // I2C 장치에 연결
    if ((fd = wiringPiI2CSetupInterface("/dev/i2c-1", deviceAddress)) < 0) {
        perror("I2C 장치 연결 실패");
        exit(1);
    }

    // I2C 장치에서 데이터 읽기
    int readValue = wiringPiI2CRead(fd);
    if (readValue < 0) {
        perror("\033[31mI2C Read Fail\033[0m\n");
    } else {
        printf("읽은 값: 0x%x\n", readValue);
        printf("\033[32mI2C Read Success\033[0m\n");
    }

    // I2C 장치에 데이터 쓰기
    int writeValue = 0x88; // 쓸 데이터
    if (wiringPiI2CWrite(fd, writeValue) < 0) {
        perror("\033[31mI2C Write Fail\033[0m\n");
    } else {
        printf("쓴 값: 0x%x\n", writeValue);
        printf("\033[32mI2C Write Success\033[0m\n");
    }

    return 0;
}
