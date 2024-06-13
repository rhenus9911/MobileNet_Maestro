#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "test.h"


int GpioCount = 0;
int PwmCount = 0;
int SPICount0 = 0;
int SPICount1 = 0;
int wifiCount = 0;
int EthernetCount = 0;
int blueCount = 0;
int n = 1;
int main() {
	for (int i = 0; i < n; i++) {
		GpioCount += GpioTest();
		PwmCount += PWMTest();
		SPICount0 += SPITest_0();
		SPICount1 += SPITest_1();
		wifiCount += wifiTest();
		EthernetCount += EthernetTest();
		blueCount += bluetoothTest();
	}
	printf("---------------------------------\n");
	printf("Test Summery\n");
	printf("---------------------------------\n");
	printf("0. GPIO               Pass: %d   Fail: %d\n", GpioCount, n - GpioCount);
	printf("1. PWM                Pass: %d   Fail: %d\n", PwmCount, n - PwmCount);
	printf("2. SPI0               Pass: %d   Fail: %d\n", SPICount0, n - SPICount0);
	printf("3. SPI0               Pass: %d   Fail: %d\n", SPICount1, n - SPICount1);
	printf("4. wifi               Pass: %d   Fail: %d\n", wifiCount, n - wifiCount);
	printf("5. Ethernet           Pass: %d   Fail: %d\n", EthernetCount, n - EthernetCount);
	printf("6. Bluetooth          Pass: %d   Fail: %d\n", blueCount, n - blueCount);
	printf("Total Test : %d", n);
	
}
