#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdint.h>

#define BUFFER_SIZE 1024

void printFile(FILE *file) {
    char buffer[BUFFER_SIZE];
    while (fgets(buffer, sizeof(buffer), file)) {
        printf("%s", buffer);
    }
}

void cpuNumCheck()
{
	FILE *cpuinfo = popen("grep processor /proc/cpuinfo | wc -l", "r");
	FILE *modelname = popen("grep Model /proc/cpuinfo", "r");
	FILE *lscpu = popen("lscpu | grep 'CPU'", "r");
	FILE *lsmem = popen("free | grep Mem | awk '{print $7}'", "r");

	if (cpuinfo == NULL || modelname == NULL || lscpu == NULL || lsmem == NULL)
	{
		printf("    [!] CPU is not run");
		exit(-1);
	}

	//printFile(lscpu);

    	char buffer[BUFFER_SIZE];
    	if (fgets(buffer, sizeof(buffer), lsmem) != NULL) {
        	int mem_avail = atoi(buffer);
        	if (mem_avail > 0)
		{
            		printf("    [+] Memory access Okay\n");
        	}
    	}

    	if (fgets(buffer, sizeof(buffer), cpuinfo) != NULL) {
        	int num_processors = atoi(buffer);
		printf("    [+] Number of processors: %d\n", num_processors);
         	if (num_processors == 4)
		{
            		printf("    [+] Processor is alright\n");
        	}
    	}
	else
	{
        	printf("    [!] Failed to read output\n");
        	exit(-1);
    	}

    	pclose(cpuinfo);
    	pclose(modelname);
    	pclose(lscpu);
    	pclose(lsmem);

}

void cpuFuncCheck()
{
	char buffer[1024];
	double cpuSpeed;
	char *ptr;
	double cpuTime;

	FILE *fp = popen("sysbench cpu --cpu-max-prime=20000 --threads=4 run", "r");
	if(fp == NULL)
	{
		printf("    [!] cpu Funciton is Failed\n");
		exit(-1);
	}
	while(fgets(buffer, sizeof(buffer), fp) != NULL)
	{
		//printf("[+] %s", buffer);
		if(strstr(buffer, "Threads started!") != NULL)
		{
			printf("    [+] %s\n", buffer);
		}
		else if((ptr = strstr(buffer, "events per second")) != NULL)
		{
			sscanf(ptr, "events per second: %lf", &cpuSpeed);
			printf("    [+] events per second: %.2lf\n", cpuSpeed);
		}
	}

	pclose(fp);
	
}
int main(int argc, char **argv)
{
	printf("[+] CPU, Processor Check\n");
	cpuNumCheck();

	printf("[+] CPU Function Check\n");
	cpuFuncCheck();
	return 0;

}
