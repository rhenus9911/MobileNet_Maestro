#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <pthread.h>
#include <sys/mman.h>
#include <linux/gpio.h>

static volatile unsigned int GPIO_PADS;
static volatile unsigned int GPIO_BASE;

#define BLOCK_SIZE (4*1024)

static volatile unsigned int *base;
static volatile unsigned int *gpio;
static volatile unsigned int *pwn;

volatile unsigned int *_wiringPiBase;
volatile unsigned int *_wiringPiGpio;

// piGpioBase
// 	The base address of the GPIO memory mapped hardware IO

#define GPIO_PERI_BASE 0xFE000000

static volatile unsigned int piGpioBase = 0;

// pinToGpio
// 	Take a Wiring pin (0 through X) and re-map it to the BCM_GPIO pin
// 	Cope for 3 different board revisions here.

static int *pinToGpio;

static int pinToGpioR1 [64] =
{
	17, 18, 21, 22, 23, 24, 25, 4,	// From the Original Wiki - GPIO 0 through 7:	wpi  0 -  7
	0,  1,				// I2C  - SDA1, SCL1				wpi  8 -  9
	8,  7,				// SPI  - CE1, CE0				wpi 10 - 11
  	10,  9, 11, 			// SPI  - MOSI, MISO, SCLK			wpi 12 - 14
  	14, 15,				// UART - Tx, Rx				wpi 15 - 16

	// Padding:

      	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,	// ... 31
  	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,	// ... 47
  	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,	// ... 63
};

static int *physToGpio;

static int physToGpioR1 [64] =
{
 	-1,		// 0
  	-1, -1,		// 1, 2
   	0, -1,
   	1, -1,
   	4, 14,
  	-1, 15,
  	17, 18,
  	21, -1,
  	22, 23,
  	-1, 24,
  	10, -1,
   	9, 25,
  	11,  8,
  	-1,  7,		// 25, 26

                                              -1, -1, -1, -1, -1,	// ... 31
  	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,	// ... 47
  	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,	// ... 63
};

bool wiringPiDebug = true;

int wiringPiSetup(void)
{
	int fd;
	
	pinToGpio = pinToGpioR1;
	physToGpio = physToGpioR1;
	piGpioBase = GPIO_PERI_BASE;
	if((fd = open("/dev/mem", O_RDWR | O_SYNC | O_CLOEXEC)) < 0)
	{
		printf("Error opening /dev/mem\n");
		exit(-1);
	}

	// set the offsets into the memory interface;
	
	GPIO_BASE = piGpioBase + 0x00200000;
	
	// GPIO
	base = NULL;
	gpio = (uint32_t *)mmap(0, BLOCK_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, GPIO_BASE);
	if(gpio == MAP_FAILED)
	{
		printf("wiringPiSetup: mmap (GPIO) failed\n");
		exit(-1);
	}

	// Export the base address for any external software that might need them
	_wiringPiBase = base;
	_wiringPiGpio = gpio;

	if(wiringPiDebug)
	{
		printf("GPIO is 0x%x\n", gpio);
		printf("wiringPi: memory map gpio 0x%x %s\n", GPIO_BASE, _wiringPiGpio ? "valid" : "invalid");
	}

	return 0;
}

int main(void)
{
	wiringPiSetup();

	return 0;
}
