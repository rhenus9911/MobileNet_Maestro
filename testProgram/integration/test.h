#ifndef TEST_H
#define TEST_H

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>
#include <wiringPiSPI.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <sys/types.h>
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <bluetooth/rfcomm.h>
#include <sys/socket.h>
#include <ctype.h>
#include <stdint.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>
#include <pthread.h>

#define GPSET0 0x1c
#define GPCLR0 0x28
#define GPLEV0 0x34
#define GPFSEL1 0x04
#define GPFSEL2 0x08
#define GPFSEL3 0x0c
#define GPSET1 0x20
#define GPCLR1 0x2c
#define GPLEV1 0x38
#define PWM_PIN18 1    // BCM_GPIO 18, Physical Pin 12
#define INPUT_PIN 7  // BCM_GPIO 4, Physical Pin 7
#define PWM_PIN12 26 // BCM 12, wipi 26
#define PWM_PIN13 23 // 13, 23
#define PWM_PIN19 24 // 19, 24
#define SPI_CHANNEL_0 0  // SPI D 0 (CE0)
#define SPI_CHANNEL_1 1  // SPI D 1 (CE1)
#define SPI_SPEED 500000 // SPI Ä (500kHz)
#define DATA_LENGTH 10
#define BUFFER_SIZE 1024
#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define YELLOW "\033[0;33m"
#define RESET "\033[0m"

#define BUFFER_SIZE 1024
#define STREAM_ARRAY_SIZE 100000000
#define NTIMES 20
#define SUCCESS_TEST 27
#define FAIL_TEST 28
#define THREADS 4
#define MEM_SIZE 1024 * 1024 * 1024
#define RANGE 1000000

typedef enum {
    LOG_SUCCESS,
    LOG_ERROR
} LogLevel;

typedef struct {
    char *message;
    LogLevel level;
} LogEntry;

typedef struct {
    int start;
    int end;
    int count;
} ThreadData;


// CPU Check
int cpuTest();
int cpuNumCheck();
int cpuPerformCheck();
void *prime_count(void *arg);
int cpuIPSCheck();
int cpuFPCheck();

// Memory
int memoryTest();
int memoryFuncCheck();
void readBandWidth();
void writeBandWidth();
void copyBandWidth();
void *write_meory(void* arg);
int memoryBandWidthCheck();
int memoryErrorCheck();

// GPIO
void setup();
void resetGPIO();
int GpioTest();
double logPWNInput();
int PWMTest();
int spi_loopback_test(int channel);
int SPITest_0();
int SPITest_1();

//Mobile
int wifiTest();
char* get_ip_address();
void check_network_interface();
void ping_test(const char* ip_adress);
void iperf_test(const char* server_ip);
int EthernetTest();
int bluetoothTest();
int i2cTest();

char *getColor(int check);
char *geResult(int check);
double timeCheck();

#endif
