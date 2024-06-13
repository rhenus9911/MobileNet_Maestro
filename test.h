#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>
#include <wiringPiSPI.h>
#include <wiringPi.h>
#include <sys/types.h>
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <bluetooth/rfcomm.h>
#include <sys/socket.h>






int GpioTest();
void setup();
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
