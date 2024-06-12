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
#define YELLOW "\033[0;33m"
#define RESET "\033[0m"

int funcCheck[5] = {0};

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

	if (cpuinfo == NULL || modelname == NULL || lscpu == NULL)
	{
		printf(RED "    [!] CPU is not run" RESET);
		exit(-1);
	}
	char buffer[BUFFER_SIZE];

	if (fgets(buffer, sizeof(buffer), cpuinfo) != NULL)
	{
		int num_processors = atoi(buffer);
		printf("    [+] Number of processors: %d\n", num_processors);
		if (num_processors == 4)
		{
			printf("    [+] Processor is alright\n");
			funcCheck[0] = 2;
		}
		else
		{
			printf("    [!] Processor number: %d\n", num_processors);
			funcCheck[0] = 0;
		}
	}
	else
	{
		printf(RED "    [!] Failed to read output\n" RESET);
		funcCheck[0] = 0;
		exit(-1);
	}

	pclose(cpuinfo);
	pclose(modelname);
	pclose(lscpu);
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
			printf("    [+] events per second: %.2lf\n", cpuSpeed);
		}
	}
	if(cpuSpeed >= 2000) funcCheck[1] = 2;
	else if(cpuSpeed >= 500) funcCheck[1] = 1;
	else funcCheck[1] = 0;

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

	if(result >= 1.0) funcCheck[2] = 2;
	else if(result >= 0.5) funcCheck[2] = 1;
	else funcCheck[2] = 0;
}

void cpuFPCheck()
{
        double start, end;

    double a = 1.234567, b = 9.87654;
    double c, sum = 0.0;

    long long num = 1000000000;

    start = timeCheck();

    for(long long i = 0; i < num; i++)
    {
        c = a * b;
        sum += c;
    }

    end = timeCheck();

    double time_stemp = end - start;

    double flops = (num * 3.0) / time_stemp / 1e6;

        printf("    [+] FLOPS: %.2lf GFLOPS\n", flops);

        if(flops >= 24) funcCheck[3] = 2;
        else if(flops >= 18) funcCheck[3] = 1;
        else funcCheck[3] = 0;
}

char* getColor(int check)
{
	if(check == 0) return RED;
	else if(check == 1) return YELLOW;
	else return GREEN;
}

char* getResult(int check)
{
	if(check == 0) return "FAIL";
	else if(check == 1) return "WARN";
	else return "PASS";
}

void printSummary()
{
	char *color;
	char *result;

	printf("=======================================================\n\n");
	printf(" Index  Test discription                        Result \n\n");
	printf("=======================================================\n\n");

	for(int i=0;i<4;i++)
	{
			if(i == 0) printf("     %d. CPU,Processor                            ", i);
			else if(i == 1) printf("     %d. CPU Performance                          ", i);
			else if(i == 2) printf("     %d. CPU IPS                                  ", i);
			else if(i == 3) printf("     %d. CPU Floating Point                       ", i);
			color = getColor(funcCheck[i]);
			result = getResult(funcCheck[i]);
			printf("%s%s%s\n\n", color, result, RESET);
	}

	printf("################### Finish Iteration ##################\n");
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

	printSummary();

	return 0;
}
