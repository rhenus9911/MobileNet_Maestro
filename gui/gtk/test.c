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

	char *log_save = malloc(2048);
	memset(log_save, 0, 2048);
	char buffer[BUFFER_SIZE];
	LogEntry result;

	if(cpuinfo == NULL || modelname == NULL)
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
	if(cpuSpeed >= 1000)
	{

		strcat(log_save, "[SUCCESS] CPU Perform Check Clear\n");
		result.level = LOG_SUCCESS;
	}
	else
	{
		strcat(log_save, "[ERROR] CPU Perform Check Failed\n");
		result.level = LOG_ERROR;
	}

	result.message = log_save;

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

	sprintf(ips_str, "[LOG] IPS: %.2lf GIPS\n", res);
	strcat(log_save, ips_str);

	if(res >= 0.5)
	{
		strcat(log_save, "[SUCCESS] CPU IPS Check Clear\n");
		result.level = LOG_SUCCESS;
	}
	else
	{
		strcat(log_save, "[ERROR] CPU IPS Check Failed\n");
		result.level = LOG_ERROR;
	}

	result.message = log_save;
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
	sprintf(fps_str, "[LOG] FLOPS: %.2lf GFLOPS\n", flops);
	strcat(log_save, fps_str);

	
	if(flops >= 18)
	{
		strcat(log_save, "[SUCCESS] CPU FP Check Clear\n");
		result.level = LOG_SUCCESS;
	}
	else
	{
		strcat(log_save, "[SUCCESS] CPU FP Check Failed\n");
		result.level = LOG_ERROR;
	}

	result.message = log_save;
	return result;
}

LogEntry memoryFuncCheck()
{
	char *log_save = malloc(2048);
	memset(log_save, 0, 2048);
	LogEntry result;

	char buffer[1024];
	char *ptr;
	double oper;
	double trans;

	FILE* lsmem = popen("free | grep Mem | awk '{print $7}'", "r");
	FILE* fp = popen("sysbench memory --threads=4 run", "r");
	if(lsmem == NULL)
	{
		strcat(result.message, "[ERROR] Memory is not Access\n");
		result.level = LOG_ERROR;
		return result;
	}
	
	while(fgets(buffer, sizeof(buffer), fp) != NULL)
	{
		if((ptr = strstr(buffer, "Total operations")) != NULL)
		{
			char oper_str[1024];
			sscanf(ptr, "Total operations: %lf", &oper);
			sprintf(oper_str, "[LOG] Total operations: %.2lf\n", oper);
			strcat(log_save, oper_str);
		}
		else if((ptr = strstr(buffer, "transferred")) != NULL)
		{
			char trans_str[1024];
			sscanf(ptr, "transferred (%lf MiB/sec)", &trans);
			sprintf(trans_str, "[LOG] MiB Transferred: %.2lf MiB/sec\n", trans);
			strcat(log_save, trans_str);
		}
	}

	if(oper >= 5000000)
	{
		strcat(log_save, "[SUCCESS] Memory is Access\n");
		result.level = LOG_SUCCESS;
	}
	else
	{
		strcat(log_save, "[ERROR] Memory Func Check Failed\n");
		result.level = LOG_ERROR;
	}

	result.message = log_save;
	pclose(fp);
	pclose(lsmem);

	return result;
}

void readBandWidth()
{
    volatile double value;
    for (size_t i = 0; i < STREAM_ARRAY_SIZE; i++)
    {
        value = array[i];
    }
}

void writeBandWidth()
{
    for (size_t i = 0; i < STREAM_ARRAY_SIZE; i++)
    {
        array[i] = i;
    }
}

void copyBandWidth()
{
    for (size_t i = 0; i < STREAM_ARRAY_SIZE; i++)
    {
        b[i] = a[i];
    }
}

LogEntry memoryBandWidthCheck()
{
	char *log_save = malloc(2048);
	memset(log_save, 0, 2048);

	LogEntry result;

	double times[3][NTIMES];
	double avg_t[3], min_t[3], max_t[3], best_rate[3];
	array = (double*)malloc(STREAM_ARRAY_SIZE * sizeof(double));

	for(int k=0;k<STREAM_ARRAY_SIZE;k++)
	{
		a[k] = 1.0;
		b[k] = 0.0;
		array[k] = 0.0;
	}

	for (int k = 0; k < NTIMES; k++)
	{
		double start = timeCheck();
		copyBandWidth();
		double end = timeCheck();
		times[0][k] = end - start;
	}
	for (int k = 0; k < NTIMES; k++)
	{
		double start = timeCheck();
		readBandWidth();
		double end = timeCheck();
		times[1][k] = end - start;
	}

	for (int k = 0; k < NTIMES; k++)
	{
		double start = timeCheck();
		writeBandWidth();
		double end = timeCheck();
		times[2][k] = end - start;
	}

	for (int i = 0; i < 3; i++)
	{
		min_t[i] = times[i][0];
		max_t[i] = times[i][0];
		avg_t[i] = 0.0;
		for (int k = 0; k < NTIMES; k++)
		{
			if (min_t[i] > times[i][k]) min_t[i] = times[i][k];
			if (max_t[i] < times[i][k]) max_t[i] = times[i][k];
			avg_t[i] += times[i][k];
		}
		
		avg_t[i] /= NTIMES;
		best_rate[i] = (2.0 * STREAM_ARRAY_SIZE * sizeof(double)) / (min_t[i] * 1.0e6);
	}
	
	char band_str[1024];
	strcat(log_save, "[LOG] Function    Best Rate MB/s  Avg time    Min time    Max time\n");
	sprintf(band_str, "[LOG] Copy:   %11.2lf         %8.6lf    %8.6lf    %8.6lf\n", best_rate[0], avg_t[0], min_t[0], max_t[0]);
	strcat(log_save, band_str);
	sprintf(band_str, "[LOG] Read:   %11.2lf         %8.6lf    %8.6lf    %8.6lf\n", best_rate[1], avg_t[1], min_t[1], max_t[1]);
	strcat(log_save, band_str);
	sprintf(band_str, "[LOG] Write:  %11.2lf         %8.6lf    %8.6lf    %8.6lf\n", best_rate[2], avg_t[2], min_t[2], max_t[2]);
	strcat(log_save, band_str);

	free(array);

	if (best_rate[1] >= 3000 && best_rate[2] >= 800 && best_rate[0] >= 700)
	{
		strcat(log_save, "[SUCCESS] Memory Bandwidth Check Clear\n");
		result.level = LOG_SUCCESS;
	}
	else
	{
		strcat(log_save, "[ERROR] Memory Bandwidth Check Fail\n");
		result.level = LOG_ERROR;
	}

	result.message = log_save;
	return result;
}

LogEntry memoryErrorCheck()
{
}


