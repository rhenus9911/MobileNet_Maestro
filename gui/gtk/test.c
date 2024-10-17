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
		strcat(log_save, "[ERROR] CPU FP Check Failed\n");
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
		strcat(result.message, "[LOG] Memory is not Access\n");
		strcat(result.message, "[ERROR] Memory Func Check Fail\n");
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
	char *log_save = malloc(2048);
	memset(log_save, 0, 2048);

	uint8_t p = 0xAA;
	uint8_t* memory = (uint8_t*)malloc(BUFFER_SIZE * sizeof(uint8_t));
	LogEntry result;

	if(!memory)
	{
		strcat(result.message, "[LOG] Memory allocation failed\n");
		strcat(result.message, "[ERROR] Memory Error Check Fail\n");
		result.level = LOG_ERROR;
		return result;
	}
	
	for(size_t i=0; i<BUFFER_SIZE;i++)
	{
		memory[i] = p;
	}
	for(size_t i=0;i<BUFFER_SIZE;i++)
	{
		if(memory[i] != p)
		{
			char mem_str[1024];
			sprintf(mem_str, "[LOG] Memory error at index %zu: expected 0x%02X, got 0x%02X\n", i, p, memory[i]);
			strcat(log_save, "[ERROR] Memory error Check Fail\n");
			strcat(result.message, log_save);
			result.level = LOG_ERROR;

			return result;
		}

	}
	
	strcat(log_save, "[SUCCESS] Memory error Check Clear\n");
	result.message = log_save;
	result.level = LOG_SUCCESS;

	return result;
}

void resetGPIO()
{
	int pins[11] = { 0, 2, 3, 25, 4, 5, 6, 31, 27, 28, 29 };
	int HighPin[5] = { 7, 21, 22, 15, 16 };
	for (int i = 0; i < 11; i++)
	{
		pinMode(pins[i], INPUT);
		digitalWrite(pins[i], LOW);
	}
	for (int i = 0; i < 5; i++)
	{
		pinMode(HighPin[i], INPUT);
		digitalWrite(pins[i], HIGH);
	}
	printf("GPIO reset completed.\n");
}

LogEntry GpioTest()
{
	char *log_save = malloc(2048);
	memset(log_save, 0, 2048);
	char gpio_str[1024];

	LogEntry result;

	if(wiringPiSetup() == -1)
	{
		strcat(result.message, "[LOG] wiringPi setup Failed!\n");
		strcat(result.message, "[ERROR] GPIO Test Fail\n");
		result.level = LOG_ERROR;
		resetGPIO();

		return result;
	}
	int pins[16] = { 7, 0, 2, 3, 21, 22, 25, 15, 16, 4, 5, 6, 31, 27, 28, 29 };
	for (int i = 0; i < 16; i++)
	{
		pinMode(pins[i], OUTPUT);
		digitalWrite(pins[i], HIGH);
		usleep(300000);
		if (digitalRead(pins[i]) == LOW)
		{
			sprintf(gpio_str, "[LOG] GPIO %d failed\n", pins[i]);
			strcat(log_save, gpio_str);
			strcat(log_save, "[ERROR] GPIO Test Fail\n");
			result.message = log_save;
			result.level = LOG_ERROR;
			resetGPIO();

			return result;
		}
		digitalWrite(pins[i], LOW);
		usleep(300000);
	}
	strcat(log_save, "[LOG] GPIO Success\n");
	strcat(log_save, "[SUCCESS] GPIO Test Clear\n");
	result.message = log_save;
	result.level = LOG_SUCCESS;
	resetGPIO();

	return result;
}

bool setup()
{
	if (wiringPiSetup() == -1)
	{
		return false;
	}
	pinMode(PWM_PIN18, PWM_OUTPUT);
	pinMode(PWM_PIN12, PWM_OUTPUT);
	pinMode(PWM_PIN13, PWM_OUTPUT);
	pinMode(PWM_PIN19, PWM_OUTPUT);
	pinMode(INPUT_PIN, INPUT);

	pwmSetMode(PWM_MODE_MS);
	pwmSetRange(1024);
	pwmSetClock(32);
	return true;
}

double logPWMInput(char *log_save)
{
	int value;
	int highCount = 0;
	int lowCount = 0;
	int totalCount = 1024;
	char log_str[1024];

	for (int i = 0; i < totalCount; i++)
	{
		value = digitalRead(INPUT_PIN);
		if (value == HIGH)
		{
			highCount++;
		}
		else
		{
			lowCount++;
		}
		delayMicroseconds(100);
	}
	
	sprintf(log_str, "HIGH Count: %d, LOW Count: %d\n", highCount, lowCount);
	strcat(log_save, log_str);
	log_str[0] = '\0';
	sprintf(log_str, "HIGH Percentage: %.2f%%, LOW Percentage: %.2f%%\n", (highCount / (float)totalCount) * 100, (lowCount / (float)totalCount) * 100);
	strcat(log_save, log_str);

	return highCount / (float)totalCount * 100;
}

bool PWMWriteCheck(char *log_save, int pin)
{
	char pin_str[1024];
	double n;

	if(pin == PWM_PIN12) strcat(log_save, "GPIO12 PWM Test\n");
	else if(pin == PWM_PIN13) strcat(log_save, "GPIO13 PWM Test\n");
	else if(pin == PWM_PIN18) strcat(log_save, "GPIO18 PWM TEst\n");
	else if(pin == PWM_PIN19) strcat(log_save, "GPIO19 PWM Test\n");
	pwmWrite(pin, 0);
	sprintf(pin_str, "input value : %d\n", 512);
	strcat(log_save, pin_str);
	n = logPWMInput(log_save);
	if (n >= 49 && n <= 51)
	{
		strcat(log_save, "[LOG] PWM Success\n");
		return true;
	}
	else
	{
		strcat(log_save, "[LOG] PWM Failed\n");
		return false;
	}
}

LogEntry PWMTest()
{
	double  n;
	char *log_save = malloc(2048);
	memset(log_save, 0, 2048);

	LogEntry result;
	
	if(!setup())
	{
		strcat(result.message, "[LOG] wiringPi connect Failed\n");
		strcat(result.message, "[ERROR] PWM Test Fail\n");
		result.level = LOG_ERROR;
		return result;
	}

	if(!PWMWriteCheck(log_save, PWM_PIN12))
	{
		strcat(log_save, "[ERROR] PWM Test Fail\n");
		result.message = log_save;
		result.level = LOG_ERROR;
		return result;
	}
	sleep(0.5);
	if(!PWMWriteCheck(log_save, PWM_PIN13))
        {
                strcat(log_save, "[ERROR] PWM Test Fail\n");
                result.message = log_save;
                result.level = LOG_ERROR;
                return result;
        }
	sleep(0.5);
	if(!PWMWriteCheck(log_save, PWM_PIN18))
        {
                strcat(log_save, "[ERROR] PWM Test Fail\n");
                result.message = log_save;
                result.level = LOG_ERROR;
                return result;
        }
	sleep(0.5);
	if(!PWMWriteCheck(log_save, PWM_PIN19))
        {
                strcat(log_save, "[ERROR] PWM Test Fail\n");
                result.message = log_save;
                result.level = LOG_ERROR;
                return result;
        }

	strcat(log_save, "[SUCCESS] PWM Test Clear\n");
	result.message = log_save;
	result.level = LOG_SUCCESS;
	return result;
}

bool spi_loopback_test(char *log_save, int channel)
{
	unsigned char data[DATA_LENGTH] = "HelloSPI!";
	unsigned char receivedData[DATA_LENGTH] = { 0 };
	char cn_str[1024];	

	sprintf(cn_str, "[LOG] Testing SPI Channel %d\n", channel);
	strcat(log_save, cn_str);
	cn_str[0] = '\0';

	strcat(log_save, "[LOG] Sending data: ");
	for (int i = 0; i < DATA_LENGTH; i++)
	{
		char tmp_str[1024];
		sprintf(tmp_str, "%02X ", data[i]);
		strcat(log_save, tmp_str);
	}
    	strcat(log_save, "\n");

    	if (wiringPiSPIDataRW(channel, data, DATA_LENGTH) == -1)
	{
		sprintf(cn_str, "[LOG] SPI communication failed on channel %d!\n", channel);
		strcat(log_save, cn_str);
		return false;
	}
	strcat(log_save, "[LOG] Received data: ");
	for (int i = 0; i < DATA_LENGTH; i++)
	{
		char tmp_str[1024];
		sprintf(tmp_str, "%02X ", data[i]);
		strcat(log_save, tmp_str);
		receivedData[i] = data[i];
	}
    	strcat(log_save, "\n");
	
	if (strncmp((char*)receivedData, "HelloSPI!", DATA_LENGTH) == 0)
	{
		sprintf(cn_str, "[LOG] SPI test passed on channel %d!\n", channel);
		strcat(log_save, cn_str);
		return true;
	}
	else
	{
        	sprintf(cn_str, "[LOG] SPI test failed on channel %d!\n", channel);
		strcat(log_save, cn_str);
        	return false;
	}
}

LogEntry SPITest()
{
	char *log_save = malloc(2048);
	memset(log_save, 0, 2048);
	
	LogEntry result;
	
	if (wiringPiSetup() == -1)
	{
		strcat(result.message, "[LOG] wiringPi setup failed!\n");
		strcat(result.message, "[ERROR] SPI Test Fail\n");
		result.level = LOG_ERROR;
		return result;
    	}
    	if (wiringPiSPISetup(SPI_CHANNEL_0, SPI_SPEED) == -1) {
		
		strcat(result.message, "[LOG] SPI setup failed on channel 0!\n");
		strcat(result.message, "[ERROR] SPI Test Fail\n");
		result.level = LOG_ERROR;
		return result;
    	}
	
	if (wiringPiSPISetup(SPI_CHANNEL_1, SPI_SPEED) == -1)
	{
		strcat(result.message, "[LOG] SPI setup failed on channel 1!\n");
		strcat(result.message, "[ERROR] SPI Test Fail\n");
		result.level = LOG_ERROR;
		return result;
	}
	if(!spi_loopback_test(log_save, SPI_CHANNEL_0))
	{
		strcat(log_save, "[ERROR] SPI Test Fail\n");
		result.message = log_save;
		result.level = LOG_ERROR;
		return result;
	}
	if(!spi_loopback_test(log_save, SPI_CHANNEL_1))
	{
                strcat(log_save, "[ERROR] SPI Test Fail\n");
                result.message = log_save;
                result.level = LOG_ERROR;
                return result;
        }
	strcat(log_save, "[SUCCESS] SPI Test Clear\n");
	result.message = log_save;
	result.level = LOG_SUCCESS;
	return result;
}

LogEntry wifiTest()
{
	char *log_save = malloc(2048);
	memset(log_save, 0, 2048);

	char buffer[BUFFER_SIZE];
    	FILE* fp;
    	int is_connected = 0;

	LogEntry result;

    	fp = popen("iwconfig 2>&1", "r");
    	if (fp == NULL)
	{
        	strcat(result.message, "[LOG] Failed to run iwconfig\n");
		strcat(result.message, "[ERROR] Wi-Fi Test Fail\n");
		result.level = LOG_ERROR;
       		return result;
    	}
    	while (fgets(buffer, sizeof(buffer), fp) != NULL)
	{
		if (strstr(buffer, "ESSID") != NULL)
		{
			if (strstr(buffer, "off/any") == NULL)
			{
				char buf_str[1024];
				is_connected = 1;
				strcat(log_save, "[LOG] Connected to Wi-Fi network.\n");
				sprintf(buf_str, "[LOG] %s", buffer);
				strcat(log_save, buf_str);
				break;
			}
		}
	}
    	if (is_connected)
	{
		while (fgets(buffer, sizeof(buffer), fp) != NULL)
		{
			if(buffer[0] != '\n')
			{
				char buf_str[1024];
				sprintf(buf_str, "[LOG] %s", buffer);
				strcat(log_save, buf_str);
			}
		}
		strcat(log_save, "[SUCCESS] Wi-Fi Test Clear\n");
		result.message = log_save;
		result.level = LOG_SUCCESS;
	}
	else
	{
		strcat(log_save, "[LOG] Not connected to any Wi-Fi network.\n");
		strcat(log_save, "[ERROR] Wi-Fi Test Fail\n");
		result.message = log_save;
		result.level = LOG_ERROR;
	}
	pclose(fp);
	return result;
}

char* get_ip_address(char *log_save)
{
	struct ifaddrs* ifaddr, * ifa;
    	char* ip_address = (char*)malloc(NI_MAXHOST * sizeof(char));
	char info_str[1024];
	if (ip_address == NULL)
	{
		return NULL;
	}
	int family, s;

	if (getifaddrs(&ifaddr) == -1)
	{
		strcat(log_save, "[LOG] Fail getifaddrs");
		return NULL;
	}
	for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
	{
		if (ifa->ifa_addr == NULL)
			continue;

		family = ifa->ifa_addr->sa_family;

		if (family == AF_INET)
		{ // IPv4
			s = getnameinfo(ifa->ifa_addr, sizeof(struct sockaddr_in), ip_address, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
		  	if (s != 0)
		  	{
				sprintf(info_str, "[LOG] getnameinfo() failed: %s\n", gai_strerror(s));
				strcat(log_save, info_str);
				freeifaddrs(ifaddr);
				free(ip_address);
				return NULL;
		  	}
      		  	if (strcmp(ifa->ifa_name, "eth0") == 0 || strcmp(ifa->ifa_name, "wlan0") == 0)
		  	{
				sprintf(info_str, "[LOG] Interface: %s\tAddress: %s\n", ifa->ifa_name, ip_address);
				strcat(log_save, info_str);
				freeifaddrs(ifaddr);
				return ip_address;
			}
		}
	}
   	freeifaddrs(ifaddr);
	free(ip_address);
	return NULL;
}

bool check_network_interface(char *log_save)
{
	strcat(log_save, "[LOG] Checking network interface with ifconfig...\n");
	int result = system("ifconfig eth0");
	if (result != 0)
	{
	strcat(log_save, "[LOG] Failed to execute ifconfig.\n");
		return false;
	}
	return true;
}


bool ping_test(const char* ip_address, char *log_save)
{
	char cmd_str[1024];
	char command[256];
	snprintf(command, sizeof(command), "ping -c 4 %s", ip_address);
	sprintf(cmd_str, "[LOG] Pinging %s...\n", ip_address);
	strcat(log_save, cmd_str);
    	int result = system(command);
	if (result != 0)
	{
		strcat(log_save, "[LOG] Ping test failed.\n");
		return false;
	}
	return true;
}

bool iperf_test(const char* server_ip, char *log_save)
{
	char cmd_str[1024];
	char command[256];
	snprintf(command, sizeof(command), "iperf3 -c %s", server_ip);
	sprintf(cmd_str, "LOG] Testing network speed with iperf3 to server %s...\n", server_ip);
	strcat(log_save, cmd_str);
	int result = system(command);
	if (result != 0)
	{
		strcat(log_save, "[LOG] iperf3 test failed.\n");
		return false;
	}
	return true;
}

LogEntry ethernetTest()
{
	char *log_save = malloc(2048);
	memset(log_save, 0, 2048);

	LogEntry result;
	
	if(!check_network_interface(log_save))
	{
		strcat(log_save, "[ERROR] Ethernet Test Fail\n");
		result.message = log_save;
		result.level = LOG_ERROR;
		return result;
	}
	const char* external_ip = "8.8.8.8";
	if(!ping_test(external_ip, log_save))
	{
		strcat(log_save, "[ERROR] Ethernet Test Fail\n");
		result.message = log_save;
		result.level = LOG_ERROR;
		return result;
	}
    	char* raspberry_pi_ip = get_ip_address(log_save);
	if (raspberry_pi_ip != NULL)
	{
		if(!iperf_test(raspberry_pi_ip, log_save))
		{
			strcat(log_save, "[ERROR] Ethernet Test Fail\n");

		}
		else
		{
			strcat(log_save, "[SUCCESS] Ethernet Test Clear\n");
		}
		result.message = log_save;
	        result.level = LOG_ERROR;
		free(raspberry_pi_ip);
		return result;
	}
	else
	{
		strcat(log_save, "[LOG] Failed to retrieve Raspberry Pi IP address.\n");
		strcat(log_save, "[ERROR] Ethernet Test Fail\n");
		result.message = log_save;
		result.level = LOG_ERROR;
		return result;
	}
}

LogEntry bluetoothTest()
{
	char *log_save = malloc(2048);
	memset( log_save, 0, 2048);

	LogEntry result;
	inquiry_info* ii = NULL;
	int max_rsp, num_rsp;
	int dev_id, sock, len, flags;
	char addr[19] = { 0 };
	char name[248] = { 0 };
	int connected_devices = 0;
    
	strcat(log_save, "[LOG] Bluetooth test Start\n");
    
	dev_id = hci_get_route(NULL);
	if (dev_id < 0)
	{
		strcat(log_save, "[LOG] hci_get_route\n");
		strcat(log_save, "[ERROR] Bluetooth Test Fail\n");
		result.message = log_save;
		result.level = LOG_ERROR;
		return result;
	}
	
	sock = hci_open_dev(dev_id);
	if (sock < 0)
	{
		strcat(log_save, "[LOG] hci_open_dev\n");
		strcat(log_save, "[LOG] Bluetooth Test Fail\n");
		result.message = log_save;
		result.level = LOG_ERROR;
		return result;
    	}

	len = 8;
	max_rsp = 255;
	flags = IREQ_CACHE_FLUSH;
	ii = (inquiry_info*)malloc(max_rsp * sizeof(inquiry_info));

	num_rsp = hci_inquiry(dev_id, len, max_rsp, NULL, &ii, flags);
	if (num_rsp < 0)
	{
		strcat(log_save, "[LOG] hci_inquiry\n");
                strcat(log_save, "[LOG] Bluetooth Test Fail\n");
                result.message = log_save;
                result.level = LOG_ERROR;
                return result;
	}
	
	for (int i = 0; i < num_rsp; i++)
	{
		ba2str(&(ii + i)->bdaddr, addr);
		memset(name, 0, sizeof(name));
		if (hci_read_remote_name(sock, &(ii + i)->bdaddr, sizeof(name), name, 0) < 0)
			strcpy(name, "[unknown]");
		
		// Check if the device is connected
		struct sockaddr_rc addr_rc = { 0 };
		int status;
		int rfcomm_sock = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
		addr_rc.rc_family = AF_BLUETOOTH;
		addr_rc.rc_channel = 1;
		str2ba(addr, &addr_rc.rc_bdaddr);

		status = connect(rfcomm_sock, (struct sockaddr*)&addr_rc, sizeof(addr_rc));
		if (status == 0)
		{
			char conn_str[1024];
			sprintf(conn_str, "[LOG] Device Adadress: %s\n", addr);
			strcat(log_save, conn_str);
			conn_str[0] = '\0';
			sprintf(conn_str, "[LOG] Device Name: %s\n", name);
			strcat(log_save, conn_str);
			strcat(log_save, "Device is connected.\n");
			connected_devices++;
		}
		close(rfcomm_sock);
	}
	if (connected_devices == 0)
		strcat(log_save, "[LOG] No connected devices.\n");
	free(ii);
	close(sock);

	strcat(log_save, "[SUCCESS] Bluetooth Test Clear\n");
	result.message = log_save;
	result.level = LOG_ERROR;

	return result;
}

LogEntry i2cTest()
{
	char *log_save = malloc(2048);
	memset(log_save, 0, 2048);
	LogEntry result;
	
	int fd;
	int deviceAddress = 0x27;
    	if ((fd = wiringPiI2CSetupInterface("/dev/i2c-1", deviceAddress)) < 0)
	{
		strcat(result.message, "[LOG] I2C ERROR\n");
		strcat(result.message, "[ERROR] I2C Test Fail\n");
		result.level = LOG_ERROR;
        	return result;
	}

    	int readValue = wiringPiI2CRead(fd);
	if (readValue < 0) {
		strcat(result.message, "[LOG] I2C Read Fail\n");
		result.level = LOG_ERROR;
		return result;
	}
    	else
	{
		char read_str[1024];
		sprintf(read_str, "[LOG] Read Data: 0x%x\n", readValue);
		strcat(log_save, read_str);
		strcat(log_save, "[LOG] I2C Read Success\n");
	}

     	int writeValue = 0x88; // ø pt0
    	if (wiringPiI2CWrite(fd, writeValue) < 0)
	{
        	strcat(log_save, "[LOG] I2C Write Fail\n");
		strcat(log_save, "[ERROR] I2C Test Fail\n");
		result.message = log_save;
		result.level = LOG_ERROR;
		return result;
	}
    	else
	{
		char write_str[1024];
		sprintf(write_str, "[LOG] Wirte Data: 0x%x\n", writeValue);
		strcat(log_save, write_str);
		strcat(log_save, "[SUCCESS] I2C Write Success");
		result.message = log_save;
		result.level = LOG_SUCCESS;
		return result;
	};
}
