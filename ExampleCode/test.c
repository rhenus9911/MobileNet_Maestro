#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define _GNU_SOURCE

int main(int argc, char **argv)
{
	
	FILE *cpuinfo=popen("cat /proc/cpuinfo | grep processor | wc -l", "rb");
	char buff[100];
	memset(buff,0x00,sizeof(buff));
	printf("%s",buff);
	if(buff[0]=='4'){
		printf("cpu is alright");
		
	}
	else
		printf("cpu is borken");
	pclose(cpuinfo);
	return 0;
}

