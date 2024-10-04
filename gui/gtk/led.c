#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "led.h"

void control_led(int a, char *result) {
	if(a == 1) {
		strcpy(result, "This is cpu");
	}
	else if(a == 2) {
		strcpy(result, "This is GPIO");
	}

	else if(a == 3) {
		strcpy(result, "This is I2C/SPI");
	}

	else {
		strcpy(result, "THis is Mobile");
	}
}
