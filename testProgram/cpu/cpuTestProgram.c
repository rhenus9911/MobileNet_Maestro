#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdint.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>

#define BUFFER_SIZE 1024
#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define RESET "\033[0m"

double timeCheck()
{
    struct timeval tp;
    gettimeofday(&tp, NULL);
    return (double) tp.tv_sec + (double) tp.tv_usec * 1.e-6;
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
    char buffer[BUFFER_SIZE];
    if (fgets(buffer, sizeof(buffer), lsmem) != NULL)
    {
        int mem_avail = atoi(buffer);
        if (mem_avail > 0)
		{
            printf(GREEN "    [+] Memory access Okay\n" RESET);
        }
    }

    if (fgets(buffer, sizeof(buffer), cpuinfo) != NULL)
    {
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

void cpuPerformCheck()
{
	char buffer[1024];
	double cpuSpeed;
	char *ptr;
	double cpuTime;

	FILE *fp = popen("sysbench cpu --cpu-max-prime=20000 --threads=4 run", "r");
	if(fp == NULL)
	{
		printf(RED "    [!] cpu Funciton is Failed\n" RESET);
		exit(-1);
	}
	while(fgets(buffer, sizeof(buffer), fp) != NULL)
	{
		if((ptr = strstr(buffer, "events per second")) != NULL)
		{
			sscanf(ptr, "events per second: %lf", &cpuSpeed);
			printf(GREEN "    [+] events per second: %.2lf\n" RESET, cpuSpeed);
		}
	}

	pclose(fp);
	
}

void cpuIPSCheck()
{
	clock_t start, end;
	double cpu_time;
	long long int instructions = 10006964235;
	int loop = 1000000000;
	double result = 0.0;

	start = timeCheck();
	for(int i=0;i<loop;i++){}
	end = timeCheck();

	cpu_time = ((double)(end-start)) / CLOCKS_PER_SEC;
	result = instructions / cpu_time;
	result /= 1000000000;

	printf("    [+] IPS: %.2lf GIPS\n", result);
}

void cpuFPCheck()
{
	double a = 1.1, b = 2.2, c = 3.3;
	long num = 100000000;
	clock_t start, end;
	double cpu_time, result = 0.0;

	start = timeCheck();
	for(long i = 1;i<num;i++)
    {
		result += sin(a) * cos(b) / tan(c);
		result += log(a) * exp(b) / sqrt(c);
		a += 0.1;
		b += 0.1;
		c += 0.1;
	}
	end = timeCheck();

	cpu_time = ((double)(end-start)) / CLOCKS_PER_SEC;

	double flops = num / cpu_time;
	flops /= 1000;

	printf("    [+] FLOPS: %.2lf MFLOPS\n", flops);
}	

int main(int argc, char **argv)
{
	printf("[+] CPU, Processor Check\n");
	cpuNumCheck();

	printf("[+] CPU Performance Check\n");
	cpuPerformCheck();

	printf("[+] CPU IPS Check\n");
	cpuIPSCheck();

	printf("[+] CPU Floating Point Check\n");
	cpuFPCheck();
	
	printf("[+] CPU Clear\n");

	return 0;

}
