#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "test.h"

char* print_sample(int num) {
	int i;
	char *output = malloc(512);
	if (output == NULL)
		return NULL;
	output[0] = '\0';
	if(num == 1) {
		for(i=0;i<10;i++) {
			strcat(output, "This is cpu log\n");

		}
	}
	else {
		strcat(output, "This is not CPU\n");
	}
	if(i == 10) {
		strcat(output, "Result is Success\n");
	}
	else {
		strcat(output, "Result is Failed\n");
	}
	
	return output;
}
