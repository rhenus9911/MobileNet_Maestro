#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <stdio.h>
#include <string.h>

#define SPI_CHANNEL_0 0  // SPI 채널 0 (CE0)
#define SPI_CHANNEL_1 1  // SPI 채널 1 (CE1)
#define SPI_SPEED 500000 // SPI 속도 (500kHz)

// 테스트할 데이터 길이
#define DATA_LENGTH 10

void spi_loopback_test(int channel) {
    unsigned char data[DATA_LENGTH] = "HelloSPI!";
    unsigned char receivedData[DATA_LENGTH] = {0};
    
    printf("Testing SPI Channel %d\n", channel);
    printf("Sending data: ");
    for(int i = 0; i < DATA_LENGTH; i++){
        printf("%02X ", data[i]);
    }
    printf("\n");
    

    // SPI로 데이터 전송 및 수신
    if (wiringPiSPIDataRW(channel, data, DATA_LENGTH) == -1) {
        printf("SPI communication failed on channel %d!\n", channel);
        return;
    }

    
    printf("Received data: ");
    for (int i = 0; i < DATA_LENGTH; i++) {
        printf("%02X ", data[i]);
        receivedData[i] = data[i];
    }
    printf("\n");

    
    if (strncmp((char *)receivedData, "HelloSPI!", DATA_LENGTH) == 0) {
        printf("SPI test passed on channel %d!\n", channel);
    } else {
        printf("SPI test failed on channel %d!\n", channel);
    }
}

int main(void) {
    // wiringPi 초기화
    if (wiringPiSetup() == -1) {
        printf("wiringPi setup failed!\n");
        return 1;
    }

    // SPI 채널 초기화
    if (wiringPiSPISetup(SPI_CHANNEL_0, SPI_SPEED) == -1) {
        printf("SPI setup failed on channel 0!\n");
        return 1;
    }

    if (wiringPiSPISetup(SPI_CHANNEL_1, SPI_SPEED) == -1) {
        printf("SPI setup failed on channel 1!\n");
        return 1;
    }

    // 채널 0 (CE0) 테스트
    spi_loopback_test(SPI_CHANNEL_0);

    // 채널 1 (CE1) 테스트
    spi_loopback_test(SPI_CHANNEL_1);

    return 0;
}
