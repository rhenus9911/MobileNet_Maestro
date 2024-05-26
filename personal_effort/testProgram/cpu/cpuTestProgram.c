#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdint.h>

#define BUFFER_SIZE 1024
#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define RESET "\033[0m"

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
		printf(RED "    [!] CPU is not run" RESET);
		exit(-1);
	}

	//printFile(lscpu);

    	char buffer[BUFFER_SIZE];
    	if (fgets(buffer, sizeof(buffer), lsmem) != NULL) {
        	int mem_avail = atoi(buffer);
        	if (mem_avail > 0)
		{
            		printf(GREEN "    [+] Memory access Okay\n" RESET);
        	}
    	}

    	if (fgets(buffer, sizeof(buffer), cpuinfo) != NULL) {
        	int num_processors = atoi(buffer);
		printf(GREEN "    [+] Number of processors: %d\n" RESET, num_processors);
         	if (num_processors == 4)
		{
            		printf(GREEN "    [+] Processor is alright\n" RESET);
        	}
    	}
	else
	{
        	printf(RED "    [!] Failed to read output\n" RESET);
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
		printf(GREEN "    [!] cpu Funciton is Failed\n" RESET);
		exit(-1);
	}
	while(fgets(buffer, sizeof(buffer), fp) != NULL)
	{
		//printf("[+] %s", buffer);
		if((ptr = strstr(buffer, "events per second")) != NULL)
		{
			sscanf(ptr, "events per second: %lf", &cpuSpeed);
			printf(GREEN "    [+] events per second: %.2lf\n" RESET, cpuSpeed);
		}
	}

	pclose(fp);
	
}

void memoryFuncCheck()
{
	char buffer[1024];
	char *ptr;
	double oper;
	double trans;
	
	FILE *fp = popen("sysbench memory --threads=4 run", "r");
	while(fgets(buffer, sizeof(buffer), fp) != NULL)
	{
		if((ptr = strstr(buffer, "Total operations")) != NULL)
		{
			sscanf(ptr, "Total operations: %lf", &oper);
			printf(GREEN "    [+] Total operations: %.2lf\n" RESET, oper);
		}
		else if((ptr = strstr(buffer, "transferred")) != NULL)
		{
			sscanf(ptr, "transferred (%lf MiB/sec)", &trans);
			printf(GREEN "    [+] MiB Transferred: %.2lf MiB/sec\n" RESET, trans);
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
	
	printf("[+] CPU Clear\n");

	printf("[+] Memory Function Check\n");
	memoryFuncCheck();

	printf("[+] Memory Clear\n");
	return 0;

}
