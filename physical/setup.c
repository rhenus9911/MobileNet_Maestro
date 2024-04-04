#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <stdint.h>
#include <sys/mman.h>
#include <unistd.h>

#define GPIO_PERI_BASE 0xFE000000
#define GPIO_BASE (GPIO_PERI_BASE + 0x200000)
#define BLOCK_SIZE 1024 * 4

#define INPUT 0
#define OUTPUT 1
#define LOW 0
#define HIGH 1

// Mask the bottom 64 pins whitch belong to the Raspberry Pi

#define PI_GPIO_MASK (0xFFFFFFC0)

#define GPSET 0x1C
#define GPCLR 0x28

static volatile unsigned int *gpio;

static uint8_t gpioToGPFSEL [] =
{
  0,0,0,0,0,0,0,0,0,0,
  1,1,1,1,1,1,1,1,1,1,
  2,2,2,2,2,2,2,2,2,2,
  3,3,3,3,3,3,3,3,3,3,
  4,4,4,4,4,4,4,4,4,4,
  5,5,5,5,5,5,5,5,5,5,
};

static uint8_t gpioToShift [] =
{
  0,3,6,9,12,15,18,21,24,27,
  0,3,6,9,12,15,18,21,24,27,
  0,3,6,9,12,15,18,21,24,27,
  0,3,6,9,12,15,18,21,24,27,
  0,3,6,9,12,15,18,21,24,27,
  0,3,6,9,12,15,18,21,24,27,
};

static int *pinToGpio;

static int gpioPin[64] = 
{
	17, 18, 21, 22, 23, 24, 25, 4,		// From the Original Wiki - GPIO 0 through 7:	wpi: 0 - 7
	0, 1,					// I2C - SDA1, SCL1				wpi: 8 - 9
	8, 7,					// SPI - CE1, CE0				wpi: 10 - 11
	10, 9, 11,				// SPI - MOSI, MISO, SCLK			wpi: 12 - 14
	14, 15,					// UART - Tx, Rx				wpi: 15 - 16
	
	// Padding:
	    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,	// ... 31
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,	// ... 47
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,	// ... 63
};

void wiringPiSetup()
{
	int fd;
	if((fd = open("/dev/mem", O_RDWR | O_SYNC | O_CLOEXEC)) < 0)
	{
		printf("Error opening /dev/mem\n");
		exit(-1);
	}

	// GPIO
	gpio = (uint32_t *)mmap(0, BLOCK_SIZE,
			PROT_READ | PROT_WRITE,
			MAP_SHARED,
			fd,
			GPIO_BASE);

	if(gpio == MAP_FAILED)
	{
		printf("wiringPiSetup: mmap (GPIO) filed\n");
		exit(-1);
	}
	printf("[+] 0x%x\n", gpio);
}

void pinMode(int pin, int mode)
{
	pin = pinToGpio[pin];
	int fSel = gpioToGPFSEL[pin];
	int shift = gpioToShift[pin];
	
	if(mode == INPUT) *(gpio+fSel) = (*(gpio+fSel) & ~(7 << shift));
	else if(mode == OUTPUT) *(gpio+fSel) = (*(gpio+fSel) & ~(7 << shift) | ( 1 << shift));

	printf("[+] pin: %d\n", pin);
	printf("[+] data: %b\n", *(gpio+fSel));
}

int main()
{
	int pin = 16;
	wiringPiSetup();
	pinMode(pin, OUTPUT);
}


