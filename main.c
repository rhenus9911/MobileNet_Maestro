#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "test.h"



int n = 1;
int m;

int main() {
	printf("0: All Test 1: CPU Test 2: GPIO Test 3: I2C/SPI Test 4: mobile Test 5: Exit\n");
	while (1) {
		int GpioCount = 0;
		int PwmCount = 0;
		int SPICount0 = 0;
		int SPICount1 = 0;
		int wifiCount = 0;
		int EthernetCount = 0;
		int blueCount = 0;
		int i2cCount = 0;
		printf("Test Num Select: ");
		scanf("%d", &m);
		if (m == 0) {
			for (int i = 0; i < n; i++) {
				GpioCount += GpioTest();
				resetGPIO();
				PwmCount += PWMTest();
				SPICount0 += SPITest_0();
				SPICount1 += SPITest_1();
				wifiCount += wifiTest();
				EthernetCount += EthernetTest();
				blueCount += bluetoothTest();
				i2cCount += i2cTest();
			}
			printf("---------------------------------\n");
			printf("Test Summery\n");
			printf("---------------------------------\n");
			printf("0. GPIO               Pass: %d   Fail: %d\n", GpioCount, n - GpioCount);
			printf("1. PWM                Pass: %d   Fail: %d\n", PwmCount, n - PwmCount);
			printf("2. SPI0               Pass: %d   Fail: %d\n", SPICount0, n - SPICount0);
			printf("3. SPI1               Pass: %d   Fail: %d\n", SPICount1, n - SPICount1);
			printf("4. I2C                Pass: %d   Fail: %d\n", i2cCount, n - i2cCount);
			printf("5. wifi               Pass: %d   Fail: %d\n", wifiCount, n - wifiCount);
			printf("6. Ethernet           Pass: %d   Fail: %d\n", EthernetCount, n - EthernetCount);
			printf("7. Bluetooth          Pass: %d   Fail: %d\n", blueCount, n - blueCount);
			printf("Total Test : %d\n", n);
		}
		else if (m == 1) {
			printf("Test start\n\n");
			cpuTest();
			memoryTest();
		}
		else if (m == 2) {
			for (int i = 0; i < n; i++) {
				GpioCount += GpioTest();
				resetGPIO();
				PwmCount += PWMTest();
			}
			printf("---------------------------------\n");
			printf("Test Summery\n");
			printf("---------------------------------\n");
			printf("0. GPIO               Pass: %d   Fail: %d\n", GpioCount, n - GpioCount);
			printf("1. PWM                Pass: %d   Fail: %d\n", PwmCount, n - PwmCount);
			printf("Total Test : %d\n", n);
		}
		else if (m == 3) {
			for (int i = 0; i < n; i++) {
				SPICount0 += SPITest_0();
				SPICount1 += SPITest_1();
				i2cCount += i2cTest();
				printf("0. SPI0               Pass: %d   Fail: %d\n", SPICount0, n - SPICount0);
				printf("1. SPI1               Pass: %d   Fail: %d\n", SPICount1, n - SPICount1);
				printf("2. I2C                Pass: %d   Fail: %d\n", i2cCount, n - i2cCount);
				printf("Total Test : %d\n", n);
			}
		}
		else if (m == 4) {
			for (int i = 0; i < n; i++) {
				wifiCount += wifiTest();
				EthernetCount += EthernetTest();
				blueCount += bluetoothTest();
				printf("0. wifi               Pass: %d   Fail: %d\n", wifiCount, n - wifiCount);
				printf("1. Ethernet           Pass: %d   Fail: %d\n", EthernetCount, n - EthernetCount);
				printf("2. Bluetooth          Pass: %d   Fail: %d\n", blueCount, n - blueCount);
				printf("Total Test : %d\n", n);

			}
		}
		else if (m == 5) break;
	}
	return 0;
}
