#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "test.h"



int n = 1;
int m;

char* get_Result(bool pass){
	if(pass){
		return "\033[32mSuccess\033[0m";
		}
	else return "\033[31mFailed\033[0m";
}

void testInit() {
    printf("----------------------------------------------\n");
	printf("Test Summery\n");
	printf("----------------------------------------------\n");
}

void printResut(int m) {
    int cnt = 0;
    if(m == 0 || m == 1) {
        
    }
    if(m == 0 || m == 2) {
        a = get_Result(GpioTest());
        printf("%d. GPIO               %s\n",cnt++, a);
        a = get_Result(PwmTest());
        printf("%d. PWM                %s\n",cnt++, a);
    }
    if(m == 0 || m == 3) {
        a = get_Result(SPITest_0());
        printf("%d. SPI0               %s\n",cnt++, a);
        a = get_Result(SPITest_1());
        printf("%d. SPI1               %s\n",cnt++, a);
        a = get_Result(i2cTest());
        printf("%d. I2C                %s\n",cnt++, a);
    }
    if(m == 0 || m == 4) {
        a = get_Result(wifiTest());
        printf("%d. wifi               %s\n",cnt++, a);
        a = get_Result(EthernetTest());
        printf("%d. Ethernet           %s\n",cnt++, a);
        a = get_Result(bluetoothTest());
        printf("%d. Bluetooth          %s\n",cnt++, a);
    }
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
				PwmCount += PWMTest();
				SPICount0 += SPITest_0();
				SPICount1 += SPITest_1();
				wifiCount += wifiTest();
				EthernetCount += EthernetTest();
				blueCount += bluetoothTest();
				i2cCount += i2cTest();
			}

            testInit();
            printResut(m);

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
			testInit();
			printResult(m);

            printf("Total Test : %d\n", n);
		}
		else if (m == 3) {
			printf("Test start\n\n");
			for (int i = 0; i < n; i++) {
				SPICount0 += SPITest_0();
				SPICount1 += SPITest_1();
				i2cCount += i2cTest();
            }
				
            testInit();
            printResult(m);

            printf("Total Test : %d\n", n);
		}
		else if (m == 4) {
			printf("Test start\n\n");
			for (int i = 0; i < n; i++) {
				wifiCount += wifiTest();
				EthernetCount += EthernetTest();
				blueCount += bluetoothTest();

            }

            testInit();
            printResult(m);

            printf("Total Test : %d\n", n);

		}
		else break;
	}
	return 0;
}