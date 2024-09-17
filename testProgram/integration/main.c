#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "test.h"

#define SUCCESS_TEST 27
#define FAIL_TEST 28

int n = 1;
int m;
bool fail = false;

char* get_Result(int pass){
	if(pass == 1){
		return "\033[32mSuccess\033[0m";
	}
	else{
		fail = true;
		return "\033[31mFailed\033[0m";
	}
}

void printResult() {

	if (wiringPiSetup() == -1) {
		printf("wiringPi is not connected\n");
		exit(1);
	}

	pinMode(SUCCESS_TEST, OUTPUT);
	pinMode(FAIL_TEST, OUTPUT);

	for(int cnt=0; cnt<5;cnt++) {
		if(!fail) {
			digitalWrite(SUCCESS_TEST, HIGH);
			delay(500);
			digitalWrite(SUCCESS_TEST, LOW);
			delay(500);
		}
		else {
			digitalWrite(FAIL_TEST, HIGH);
			delay(500);
			digitalWrite(FAIL_TEST, LOW);
			delay(500);
		}
	}
	if(fail) exit(-1);
}

int main() {
	char* a;

	fail = false;	

	while (1) {
        int CPUCount = 0;
        int MemCount = 0;
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
			for (int i = 0; i < n; i++) {
                CPUCount += cpuTest();
                MemCount += memoryTest();
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
            a = get_Result(CPUCount);
			printf("0. CPU                               %s\n", a);
			a = get_Result(MemCount);
			printf("1. Memory                            %s\n", a);
			a = get_Result(GpioCount);
			printf("2. GPIO                              %s\n", a);
			a = get_Result(PwmCount);
			printf("3. PWM                               %s\n", a);
			a = get_Result(SPICount0);
			printf("4. SPI0(A Board Only)                %s\n", a);
			a = get_Result(SPICount1);
			printf("5. SPI1(A Board Only)                %s\n", a);
			a = get_Result(i2cCount);
			printf("6. I2C                               %s\n", a);
			a = get_Result(wifiCount);
			printf("7. wifi                              %s\n", a);
			a = get_Result(EthernetCount);
			printf("8. Ethernet                          %s\n", a);
			a = get_Result(blueCount);
			printf("9. Bluetooth                         %s\n", a);
			printf("Total Test : %d\n", n);
		}
		else if (m == 1) {
			printf("Test start\n\n");

            for(int i = 0; i < n; i++)
            {
                CPUCount += cpuTest();
                MemCount += memoryTest();
            }

			printf("---------------------------------\n");
			printf("Test Summery\n");
			printf("---------------------------------\n");
			a = get_Result(CPUCount);
			printf("0. CPU                %s\n", a);
			a = get_Result(MemCount);
			printf("1. Memory             %s\n", a);

			printResult();
			printf("Total Test : %d\n", n);
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
			
			printResult();
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

				printResult();
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

				printResult();
				printf("Total Test : %d\n", n);

			}
		}
		else break;
	}
	return 0;
}
