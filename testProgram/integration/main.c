#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "test.h"



int n = 1;
int m;

char* get_Result(int pass){
	if(pass == 1){
		return "\033[32mSuccess\033[0m";
		}
	else return "\033[31mFailed\033[0m";
}


int main() {
	char* a;
	while (1) {
		int GpioCount = 0;
		int PwmCount = 0;
		int SPICount0 = 0;
		int SPICount1 = 0;
		int wifiCount = 0;
		int EthernetCount = 0;
		int blueCount = 0;
		int i2cCount = 0;
		printf("0: All Test 1: CPU Test 2: GPIO Test 3: I2C/SPI Test 4: mobile Test 5: Exit\n");
		printf("Test Num Select: ");
		scanf("%d", &m);
		if (m == 0) {
			printf("Test start\n\n");
			cpuTest();
			memoryTest();
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
			printf("----------------------------------------------\n");
			printf("Test Summery\n");
			printf("----------------------------------------------\n");
			a = get_Result(GpioCount);
			printf("0. GPIO                              %s\n", a);
			a = get_Result(PwmCount);
			printf("1. PWM                               %s\n", a);
			a = get_Result(SPICount0);
			printf("2. SPI0(A Board Only)                %s\n", a);
			a = get_Result(SPICount1);
			printf("3. SPI1(A Board Only)                %s\n", a);
			a = get_Result(i2cCount);
			printf("4. I2C                               %s\n", a);
			a = get_Result(wifiCount);
			printf("5. wifi                              %s\n", a);
			a = get_Result(EthernetCount);
			printf("6. Ethernet                          %s\n", a);
			a = get_Result(blueCount);
			printf("7. Bluetooth                         %s\n", a);
			printf("Total Test : %d\n", n);
		}
		else if (m == 1) {
			printf("Test start\n\n");
			cpuTest();
			memoryTest();
		}
		else if (m == 2) {
			printf("Test start\n\n");
			for (int i = 0; i < n; i++) {
				GpioCount += GpioTest();
				resetGPIO();
				PwmCount += PWMTest();
			}
			printf("---------------------------------\n");
			printf("Test Summery\n");
			printf("---------------------------------\n");
			a = get_Result(GpioCount);
			printf("0. GPIO               %s\n", a);
			a = get_Result(PwmCount);
			printf("1. PWM                %s\n", a);
			printf("Total Test : %d\n", n);
		}
		else if (m == 3) {
			printf("Test start\n\n");
			for (int i = 0; i < n; i++) {
				SPICount0 += SPITest_0();
				SPICount1 += SPITest_1();
				i2cCount += i2cTest();
				printf("---------------------------------\n");
				printf("Test Summery\n");
				printf("---------------------------------\n");
				a = get_Result(SPICount0);
				printf("0. SPI0               %s\n", a);
				a = get_Result(SPICount1);
				printf("1. SPI1               %s\n", a);
				a = get_Result(i2cCount);
				printf("2. I2C                %s\n", a);
				printf("Total Test : %d\n", n);
			}
		}
		else if (m == 4) {
			printf("Test start\n\n");
			for (int i = 0; i < n; i++) {
				wifiCount += wifiTest();
				EthernetCount += EthernetTest();
				blueCount += bluetoothTest();
				printf("---------------------------------\n");
				printf("Test Summery\n");
				printf("---------------------------------\n");
				a = get_Result(wifiCount);
				printf("0. wifi               %s\n", a);
				a = get_Result(EthernetCount);
				printf("1. Ethernet           %s\n", a);
				a = get_Result(blueCount);
				printf("2. Bluetooth          %s\n", a);
				printf("Total Test : %d\n", n);

			}
		}
		else break;
	}
	return 0;
}