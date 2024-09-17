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




void setup();
int GpioTest();
int logPWMInput();
int PWMTest();
int spi_loopback_test(int channel);
int SPITest_0();
int SPITest_1();
int wifiTest();
//이더넷부터
char* get_ip_address();
void check_network_interface();
void ping_test(const char* ip_address);
void iperf_test(const char* server_ip);
int EthernetTest();
int bluetoothTest();
int i2cTest();
void resetGPIO();
double timeCheck();
int cpuNumCheck();
int cpuPerformCheck();
int cpuIPSCheck();
int cpuFPCheck();
int memoryFuncCheck();
void readBandWidth();
void writeBandWidth();
void copyBandWidth();
int memoryBandWidthCheck();
int memoryErrorCheck();
char* getColor(int check);
char* getResult(int check);
int cpuTest();
int memoryTest();
