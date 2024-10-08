#include "test.h"

int funcCheck[5] = { 0 };

double* array;
static double a[STREAM_ARRAY_SIZE];
static double b[STREAM_ARRAY_SIZE];

double timeCheck()
{
	    struct timeval tp;
	        gettimeofday(&tp, NULL);
		    return (double)tp.tv_sec + (double)tp.tv_usec * 1.e-6;
}

LogEntry cpuNumCheck()
{
	FILE* cpuinfo = popen("grep processor /proc/cpuinfo | wc -l", "r");
	FILE* modelname = popen("grep Model /proc/cpuinfo", "r");
	FILE* lscpu = popen("lscpu | grep 'CPU'", "r");

	char *log_save = malloc(2048);
	memset(log_save, 0, 2048);
	char buffer[BUFFER_SIZE];
	LogEntry result;

	if(cpuinfo == NULL || modelname == NULL || lscpu == NULL)
	{
		result.message = "[ERROR] CPU is not run\n";
		result.level = LOG_ERROR;
		return result;
	}

	if(fgets(buffer, sizeof(buffer), cpuinfo) != NULL)
	{
		int num_processors = atoi(buffer);
		char processors_str[BUFFER_SIZE];
		char final_log[BUFFER_SIZE * 2];
		sprintf(processors_str, "Number of processors: %d\n", num_processors);
		sprintf(final_log, "[LOG] %s", processors_str);
		strcat(log_save, final_log);
		
		if(num_processors == 4)
		{
			strcat(log_save, "[SUCCESS] Processors is alright\n");
			result.message = log_save;
			result.level = LOG_SUCCESS;
		}
		else
		{
			sprintf(final_log, "[ERROR] Processor number: %d\n", num_processors);
			strcat(log_save, final_log);
			result.message = log_save;
			result.level = LOG_ERROR;
		}
	}
	else
	{
		result.message = "[ERROR] Failed to read output\n";
		result.level = LOG_ERROR;
	}
	pclose(cpuinfo);
	pclose(modelname);
	pclose(lscpu);
	
	return result;
}

LogEntry cpuPerformCheck()
{
	char *log_save = malloc(2048);
	memset(log_save, 0, 2048);    
	char buffer[1024];
	double cpuSpeed;
	char *ptr;
	LogEntry result;

	FILE *fp = popen("sysbench cpu --cpu-max-prime=20000 --threads=4 run", "r");
	if(fp == NULL)
	{
		result.message = "[ERROR] CPU Function is Failed\n";
		result.level = LOG_ERROR;
		return result;
	}
	while(fgets(buffer, sizeof(buffer), fp) != NULL)
	{
		if((ptr = strstr(buffer, "events per second")) != NULL)
		{
			char speed_str[BUFFER_SIZE];
			sscanf(ptr, "events per second: %lf", &cpuSpeed);
			sprintf(speed_str, "[LOG] events per second: %.2lf\n", cpuSpeed);
			strcat(log_save, speed_str);
		}
		
	}
	result.message = log_save;
	if(cpuSpeed >= 1000) result.level = LOG_ERROR;
	else result.level = LOG_SUCCESS;

	pclose(fp);
	return result;
}

LogEntry cpuIPSCheck()
{
	char *log_save = malloc(2048);
	memset(log_save, 0, 2048);    

	clock_t start, end;
	double cpu_time;
	long long int instructions = 10006964235;
	int loop = 1000000000;
	double res = 0.0;
	char ips_str[BUFFER_SIZE];
	LogEntry result;

	start = timeCheck();
	for(int i=0;i<loop;i++) {}
	end = timeCheck();
	cpu_time = ((double)(end-start)) / CLOCKS_PER_SEC;
	res = instructions / cpu_time;
	res /= 1000000000;

	sprintf(ips_str, "[LOG] IPS: %2.lf GIPS\n", res);
	strcat(log_save, ips_str);

	result.message = log_save;
	if(res >= 0.5) result.level = LOG_SUCCESS;
	else result.level = LOG_ERROR;

	return result;
}

LogEntry cpuFPCheck()
{
	char *log_save = malloc(2048);
	memset(log_save, 0, 2048);    

	double start, end;
	double a=1.234567, b=9.87654;
	double c, sum = 0.0;
	long long num = 1000000000;
	LogEntry result;

	start = timeCheck();
	for(long long i=0;i<num;i++)
	{
		c = a*b;
		sum += c;
	}
	end = timeCheck();

	double time_stamp = end - start;
	double flops = (num * 3.0) / time_stamp / 1e6;

	char fps_str[BUFFER_SIZE];
	sprintf(fps_str, "FLOPS: %.2lf GFLOPS\n", flops);
	strcat(log_save, fps_str);

	result.message = log_save;
	if(flops >= 18) result.level = LOG_SUCCESS;
	else result.level = LOG_ERROR;

	return result;
}	
