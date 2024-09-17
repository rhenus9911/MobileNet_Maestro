#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>
#include <wiringPiSPI.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <sys/types.h>
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <bluetooth/rfcomm.h>
#include <sys/socket.h>
#include <ctype.h>
#include <stdint.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>

#define GPIO_BASE 0xfe200000
#define GPFSEL0 0x00
#define GPSET0 0x1c
#define GPCLR0 0x28
#define GPLEV0 0x34
#define GPFSEL1 0x04
#define GPFSEL2 0x08
#define GPFSEL3 0x0c
#define GPSET1 0x20
#define GPCLR1 0x2c
#define GPLEV1 0x38
#define PWM_PIN18 1    // BCM_GPIO 18, Physical Pin 12
#define INPUT_PIN 7  // BCM_GPIO 4, Physical Pin 7
#define PWM_PIN12 26 // BCM 12, wipi 26
#define PWM_PIN13 23 // 13, 23
#define PWM_PIN19 24 // 19, 24
#define SPI_CHANNEL_0 0  // SPI D 0 (CE0)
#define SPI_CHANNEL_1 1  // SPI D 1 (CE1)
#define SPI_SPEED 500000 // SPI Ä (500kHz)
#define DATA_LENGTH 10
#define BUFFER_SIZE 1024
#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define YELLOW "\033[0;33m"
#define RESET "\033[0m"

#define BUFFER_SIZE 1024
#define STREAM_ARRAY_SIZE 100000000
#define NTIMES 20

#define SUCCESS 27
#define FAIL 28

int funcCheck[5] = { 0 };

double* array;
static double a[STREAM_ARRAY_SIZE];
static double b[STREAM_ARRAY_SIZE];


int GpioTest() {
    // wiringPi 0T
    if (wiringPiSetup() == -1) {
        printf("wiringPi setup failed!\n");
        return 1;
    }
    int pins[16] = { 7, 0, 2, 3, 21, 22, 25, 15, 16, 4, 5, 6, 31, 27, 28, 29 };
    int check = 1;
    for (int i = 0; i < 16; i++) {
        pinMode(pins[i], OUTPUT);
        
        digitalWrite(pins[i], HIGH);
        //if(i == 5){
            //digitalWrite(pins[i], LOW); 
            //}
        usleep(300000); // 0.3  0
        if (digitalRead(pins[i]) == LOW) {
            printf("\033[31mGPIO %d failed\033[0m\n", pins[i]);
            check = 0;
        }
        digitalWrite(pins[i], LOW);
        usleep(300000); // 0.3  0
    }
    
    if (check == 1) {
        printf("\033[32m GPIO Success\033[0m\n");
        digitalWrite(SUCCESS, HIGH);
        return 1;
        }
    else return 0;
}

void setup() {
    if (wiringPiSetup() == -1) {
        printf("wiringPi 0T ä(\n");
        exit(1);
    }

    pinMode(PWM_PIN18, PWM_OUTPUT);
    pinMode(PWM_PIN12, PWM_OUTPUT);
    pinMode(PWM_PIN13, PWM_OUTPUT);
    pinMode(PWM_PIN19, PWM_OUTPUT);
    pinMode(INPUT_PIN, INPUT);

    pwmSetMode(PWM_MODE_MS);
    pwmSetRange(1024);
    pwmSetClock(32);
}
double logPWMInput() {
    int value;
    int highCount = 0;
    int lowCount = 0;
    int totalCount = 1024; // !` Ø 

    for (int i = 0; i < totalCount; i++) {
        value = digitalRead(INPUT_PIN);

        if (value == HIGH) {
            highCount++;
        }
        else {
            lowCount++;
        }

        delayMicroseconds(100); // 100usÈä ØÁ
    }

    printf("HIGH Count: %d, LOW Count: %d\n", highCount, lowCount);
    printf("HIGH Percentage: %.2f%%, LOW Percentage: %.2f%%\n",
        (highCount / (float)totalCount) * 100,
        (lowCount / (float)totalCount) * 100);
    return highCount / (float)totalCount * 100;
}

int PWMTest() {
    setup();
    double  n;
    int check = 0;
    printf("GPIO12 PWM Test\n");
    pwmWrite(PWM_PIN12, 0);
    printf("input value : %d\n", 512);
    n = logPWMInput();
    if (n >= 49 && n <= 51) {
        printf("\033[32mPWM Success\033[0m\n");
    }
    else {
        printf("\033[31mPWM Failed\033[0m\n");
        check = 1;
    }
    sleep(1); // 1Èä \ø %

    printf("GPIO13 PWM Test\n");
    pwmWrite(PWM_PIN13, 512);
    printf("input value : %d\n", 512);
    n = logPWMInput();
    if (n >= 49 && n <= 51) {
        printf("\033[32mPWM Success\033[0m\n");
    }
    else {
        printf("\033[31mPWM Failed\033[0m\n");
        check = 1;
    }
    sleep(1); // 1Èä \ø %

    printf("GPIO18 PWM Test\n");
    pwmWrite(PWM_PIN18, 512);
    printf("input value : %d\n", 512);
    n = logPWMInput();
    if (n >= 49 && n <= 51) {
        printf("\033[32mPWM Success\033[0m\n");
    }
    else {
        printf("\033[31mPWM Failed\033[0m\n");
        check = 1;
    }
    sleep(1); // 1Èä \ø %
    printf("GPIO19 PWM Test\n");
    pwmWrite(PWM_PIN19, 512);
    printf("input value : %d\n", 512);
    n = logPWMInput();
    if (n >= 49 && n <= 51) {
        printf("\033[32mPWM Success\033[0m\n");
    }
    else {
        printf("\033[31mPWM Failed\033[0m\n");
        check = 1;
    }
    sleep(1); // 1Èä \ø %


    if (check == 1) return 0;
    else return 1;
}

int spi_loopback_test(int channel) {
    unsigned char data[DATA_LENGTH] = "HelloSPI!";
    unsigned char receivedData[DATA_LENGTH] = { 0 };

    printf("Testing SPI Channel %d\n", channel);
    printf("Sending data: ");
    for (int i = 0; i < DATA_LENGTH; i++) {
        printf("%02X ", data[i]);
    }
    printf("\n");


    // SPI\ pt0 ¡  à
    if (wiringPiSPIDataRW(channel, data, DATA_LENGTH) == -1) {
        printf("SPI communication failed on channel %d!\n", channel);
        return 0;
    }


    printf("Received data: ");
    for (int i = 0; i < DATA_LENGTH; i++) {
        printf("%02X ", data[i]);
        receivedData[i] = data[i];
    }
    printf("\n");


    if (strncmp((char*)receivedData, "HelloSPI!", DATA_LENGTH) == 0) {
        printf("\033[32mSPI test passed on channel %d!\033[0m\n", channel);
        return 1;
    }
    else {
        printf("\033[31mSPI test failed on channel %d!\033[0m\n", channel);
        return 0;
    }
}

int SPITest_0() {
    // wiringPi 0T
    if (wiringPiSetup() == -1) {
        printf("wiringPi setup failed!\n");
        return 0;
    }

    // SPI D 0T
    if (wiringPiSPISetup(SPI_CHANNEL_0, SPI_SPEED) == -1) {
        printf("SPI setup failed on channel 0!\n");
        return 0;
    }

    if (wiringPiSPISetup(SPI_CHANNEL_1, SPI_SPEED) == -1) {
        printf("SPI setup failed on channel 1!\n");
        return 0;
    }
    int n;
    // D 0 (CE0) L¤¸
    n = spi_loopback_test(SPI_CHANNEL_0);
    return n;


}

int SPITest_1() {
    // wiringPi 0T
    if (wiringPiSetup() == -1) {
        printf("wiringPi setup failed!\n");
        return 0;
    }

    // SPI D 0T
    if (wiringPiSPISetup(SPI_CHANNEL_0, SPI_SPEED) == -1) {
        printf("SPI setup failed on channel 0!\n");
        return 0;
    }

    if (wiringPiSPISetup(SPI_CHANNEL_1, SPI_SPEED) == -1) {
        printf("SPI setup failed on channel 1!\n");
        return 0;
    }


    int n;
    // D 1 (CE1) L¤¸
    n = spi_loopback_test(SPI_CHANNEL_1);
    return n;
}

int wifiTest() {
    char buffer[BUFFER_SIZE];
    FILE* fp;
    int is_connected = 0;

    fp = popen("iwconfig 2>&1", "r");
    if (fp == NULL) {
        printf("Failed to run iwconfig\n");
        return 0;
    }

    // 9 %D |Ð  ¥
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        // ESSID Ux
        if (strstr(buffer, "ESSID") != NULL) {
            if (strstr(buffer, "off/any") == NULL) {
                is_connected = 1;
                printf("\033[32mConnected to Wi-Fi network.\033[0m\n");
                printf("%s", buffer);
                break;
            }
        }
    }

    // ð° ½°   ô %
    if (is_connected) {
        while (fgets(buffer, sizeof(buffer), fp) != NULL) {
            printf("%s", buffer);
        }
        return 1;
    }
    else {
        printf("\033[31mNot connected to any Wi-Fi network.\033[0m\n");
        return 0;
    }


    pclose(fp);
}

char* get_ip_address() {
    struct ifaddrs* ifaddr, * ifa;
    char* ip_address = (char*)malloc(NI_MAXHOST * sizeof(char));
    if (ip_address == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    int family, s;

    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs");
        exit(EXIT_FAILURE);
    }


    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == NULL)
            continue;

        family = ifa->ifa_addr->sa_family;

        if (family == AF_INET) { // IPv4
            s = getnameinfo(ifa->ifa_addr, sizeof(struct sockaddr_in), ip_address, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
            if (s != 0) {
                printf("getnameinfo() failed: %s\n", gai_strerror(s));
                freeifaddrs(ifaddr);
                free(ip_address);
                exit(EXIT_FAILURE);
            }
            if (strcmp(ifa->ifa_name, "eth0") == 0 || strcmp(ifa->ifa_name, "wlan0") == 0) {
                printf("Interface: %s\tAddress: %s\n", ifa->ifa_name, ip_address);
                freeifaddrs(ifaddr);
                return ip_address;
            }
        }
    }

    freeifaddrs(ifaddr);
    free(ip_address);
    return NULL;
}


void check_network_interface() {
    printf("Checking network interface with ifconfig...\n");
    int result = system("ifconfig eth0");
    if (result != 0) {
        printf("\033[31mFailed to execute ifconfig.\033[0m\n");
    }
}


void ping_test(const char* ip_address) {
    char command[256];
    snprintf(command, sizeof(command), "ping -c 4 %s", ip_address);
    printf("Pinging %s...\n", ip_address);
    int result = system(command);
    if (result != 0) {
        printf("\033[31mPing test failed.\033[0m\n");
    }
}

void iperf_test(const char* server_ip) {
    char command[256];
    snprintf(command, sizeof(command), "iperf3 -c %s", server_ip);
    printf("Testing network speed with iperf3 to server %s...\n", server_ip);
    int result = system(command);
    if (result != 0) {
        printf("\033[31miperf3 test failed.\033[0m\n");
    }
}

int EthernetTest() {
    // 1. $¸Ìl $ Ux
    check_network_interface();

    // 2. $¸Ìl ð° L¤¸
    const char* external_ip = "8.8.8.8";
    ping_test(external_ip);

    // 3. | ¬ tX IP ü Ux  $¸Ìl Ä L¤¸
    char* raspberry_pi_ip = get_ip_address();
    if (raspberry_pi_ip != NULL) {
        iperf_test(raspberry_pi_ip);
        free(raspberry_pi_ip);
        return 1;
    }
    else {
        printf("\033[31mFailed to retrieve Raspberry Pi IP address.\033[0m\n");
        return 0;
    }
}

int bluetoothTest() {
    inquiry_info* ii = NULL;
    int max_rsp, num_rsp;
    int dev_id, sock, len, flags;
    char addr[19] = { 0 };
    char name[248] = { 0 };
    int connected_devices = 0;
    printf("bluetoothTest Start\n");
    dev_id = hci_get_route(NULL);
    if (dev_id < 0) {
        perror("hci_get_route");
        exit(1);
    }

    sock = hci_open_dev(dev_id);
    if (sock < 0) {
        perror("hci_open_dev");
        exit(1);
    }

    len = 8;
    max_rsp = 255;
    flags = IREQ_CACHE_FLUSH;
    ii = (inquiry_info*)malloc(max_rsp * sizeof(inquiry_info));

    num_rsp = hci_inquiry(dev_id, len, max_rsp, NULL, &ii, flags);
    if (num_rsp < 0) {
        perror("hci_inquiry");
        exit(1);
    }

    for (int i = 0; i < num_rsp; i++) {
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
        if (status == 0) {
            printf("Device Address: %s\n", addr);
            printf("Device Name: %s\n", name);
            printf("Device is connected.\n");
            connected_devices++;
        }
        close(rfcomm_sock);
    }

    if (connected_devices == 0) {
        printf("\033[31mNo connected devices.\033[0m\n");
        return 0;
    }
    else return 1;

    free(ii);
    close(sock);
}

int i2cTest() {
    int fd;
    int deviceAddress = 0x27; // I2C ¥X ü
    //int bus = 1; // I2C ¤ 8
    int check = 1;
    // I2C ¥XÐ ð°
    if ((fd = wiringPiI2CSetupInterface("/dev/i2c-1", deviceAddress)) < 0) {
        perror("I2C failed");
        exit(1);
    }

    // I2C ¥XÐ pt0 }0
    int readValue = wiringPiI2CRead(fd);
    if (readValue < 0) {
        perror("\033[31mI2C Read Fail\033[0m\n");
        check = 0;
        
    }
    else {
        printf("Read Data: 0x%x\n", readValue);
        printf("\033[32mI2C Read Success\033[0m\n");
        
    }

    // I2C ¥XÐ pt0 ð0
    int writeValue = 0x88; // ø pt0
    if (wiringPiI2CWrite(fd, writeValue) < 0) {
        perror("\033[31mI2C Write Fail\033[0m\n");
        check = 0;
    }
    else {
        printf("Wirte Data: 0x%x\n", writeValue);
        printf("\033[32mI2C Write Success\033[0m\n");
        
    }
    if(check == 1) return 1;
    else return 0;

}
void resetGPIO() {
    int pins[11] = { 0, 2, 3, 25, 4, 5, 6, 31, 27, 28, 29 };
    int HighPin[5] = { 7, 21, 22, 15, 16 };
    for (int i = 0; i < 11; i++) {
        pinMode(pins[i], INPUT); // ¨à @D 0ø % ¨Ü\ 0T
        digitalWrite(pins[i], LOW);
    }
    for (int i = 0; i < 5; i++) {
        pinMode(HighPin[i], INPUT); // ¨à @D 0ø % ¨Ü\ 0T
        digitalWrite(pins[i], HIGH);
    }
    printf("GPIO reset completed.\n");
}

double timeCheck()
{
    struct timeval tp;
    gettimeofday(&tp, NULL);
    return (double)tp.tv_sec + (double)tp.tv_usec * 1.e-6;
}

void cpuNumCheck()
{
    FILE* cpuinfo = popen("grep processor /proc/cpuinfo | wc -l", "r");
    FILE* modelname = popen("grep Model /proc/cpuinfo", "r");
    FILE* lscpu = popen("lscpu | grep 'CPU'", "r");

    if (cpuinfo == NULL || modelname == NULL || lscpu == NULL)
    {
        printf(RED "CPU is not run" RESET);
        exit(-1);
    }
    char buffer[BUFFER_SIZE];

    if (fgets(buffer, sizeof(buffer), cpuinfo) != NULL)
    {
        int num_processors = atoi(buffer);
        //printf("Number of processors: %d\n", num_processors);

        if (num_processors == 4)
        {
            //printf("    [+] Processor is alright\n");
            funcCheck[0] = 2;
        }
        else
        {
            //printf("[!] Processor number: %d\n", num_processors);
            funcCheck[0] = 0;
        }
    }
    else
    {
        printf(RED "[!] Failed to read output\n" RESET);
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
    char* ptr;
    //double cpuTime;

    FILE* fp = popen("sysbench cpu --cpu-max-prime=20000 --threads=4 run", "r");
    if (fp == NULL)
    {
        printf(RED "[!] cpu Funciton is Failed\n" RESET);
        exit(-1);
    }
    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        if ((ptr = strstr(buffer, "events per second")) != NULL)
        {
            sscanf(ptr, "events per second: %lf", &cpuSpeed);
            //printf("events per second: %.2lf\n", cpuSpeed);
        }
    }

    if (cpuSpeed >= 2000) funcCheck[1] = 2;
    else if (cpuSpeed >= 500) funcCheck[1] = 1;
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
    for (int i = 0; i < loop; i++) {}
    end = timeCheck();

    cpu_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    result = instructions / cpu_time;
    result /= 1000000000;

    //printf("IPS: %.2lf GIPS\n", result);

    if (result >= 1.0) funcCheck[2] = 2;
    else if (result >= 0.5) funcCheck[2] = 1;
    else funcCheck[2] = 0;
}

void cpuFPCheck()
{
    double start, end;


    double a = 1.234567, b = 9.87654;
    double c, sum = 0.0;

    long long num = 1000000000;

    start = timeCheck();

    for (long long i = 0; i < num; i++)
    {
        c = a * b;
        sum += c;
    }

    end = timeCheck();

    double time_stemp = end - start;

    double flops = (num * 3.0) / time_stemp / 1e6;

    //printf("FLOPS: %.2lf GFLOPS\n", flops);

    if (flops >= 24) funcCheck[3] = 2;
    else if (flops >= 18) funcCheck[3] = 1;
    else funcCheck[3] = 0;
}

void memoryFuncCheck()
{
    char buffer[1024];
    char* ptr;
    double oper;
    double trans;

    FILE* lsmem = popen("free | grep Mem | awk '{print $7}'", "r");
    FILE* fp = popen("sysbench memory --threads=4 run", "r");
    if (lsmem == NULL)
    {
        printf(RED "[!] Memory is not Access\n" RESET);
        exit(-1);
    }
    else
    {
        //printf("Memory is Access\n");
        funcCheck[0] = 2;
    }
    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        if ((ptr = strstr(buffer, "Total operations")) != NULL)
        {
            sscanf(ptr, "Total operations: %lf", &oper);
            //printf("Total operations: %.2lf\n", oper);
        }
        else if ((ptr = strstr(buffer, "transferred")) != NULL)
        {
            sscanf(ptr, "transferred (%lf MiB/sec)", &trans);
            //printf("MiB Transferred: %.2lf MiB/sec\n", trans);
        }
    }

    if (oper >= 20000000.0) funcCheck[1] = 2;
    else if (oper >= 5000000) funcCheck[1] = 1;
    else funcCheck[1] = 0;

    pclose(fp);
    pclose(lsmem);
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

void memoryBandWidthCheck()
{
    double times[3][NTIMES];
    double avg_t[3], min_t[3], max_t[3], best_rate[3];
    array = (double*)malloc(STREAM_ARRAY_SIZE * sizeof(double));

    for (int k = 0; k < STREAM_ARRAY_SIZE; k++)
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
    printf("Function    Best Rate MB/s  Avg time    Min time    Max time\n");
    printf("Copy:   %11.2lf         %8.6lf    %8.6lf    %8.6lf\n", best_rate[0], avg_t[0], min_t[0], max_t[0]);
    printf("Read:   %11.2lf         %8.6lf    %8.6lf    %8.6lf\n", best_rate[1], avg_t[1], min_t[1], max_t[1]);
    printf("Write:  %11.2lf         %8.6lf    %8.6lf    %8.6lf\n", best_rate[2], avg_t[2], min_t[2], max_t[2]);

    if (best_rate[1] >= 4000 && best_rate[2] >= 1600 && best_rate[0] >= 1500) funcCheck[2] = 2;
    else if (best_rate[1] < 4000 && best_rate[2] < 1600 && best_rate[0] < 1500) funcCheck[2] = 0;
    else funcCheck[2] = 1;

    free(array);
}

void memoryErrorCheck()
{
    uint8_t p = 0xAA;
    uint8_t* memory = (uint8_t*)malloc(BUFFER_SIZE * sizeof(uint8_t));
    if (!memory)
    {
        printf("[!] Memory allocation failed\n");
        exit(-1);
    }

    for (size_t i = 0; i < BUFFER_SIZE; i++)
    {
        memory[i] = p;
    }

    for (size_t i = 0; i < BUFFER_SIZE; i++)
    {
        if (memory[i] != p)
        {
            printf("[!] Memory error at index %zu: expected 0x%02X, got 0x%02X\n", i, p, memory[i]);
            funcCheck[3] = 0;
            exit(-1);
        }
    }

    funcCheck[3] = 2;
}

char* getColor(int check)
{
    if (check == 0) return RED;
    else if (check == 1) return YELLOW;
    else return GREEN;
}

char* getResult(int check)
{
    if (check == 0) return "FAIL";
    else if (check == 1) return "WARN";
    else return "PASS";
}

void printSummary()
{
    char* color;
    char* result;

    printf("=======================================================\n\n");
    printf(" Index  Test discription                        Result \n\n");
    printf("=======================================================\n\n");

    for (int i = 0; i < 4; i++)
    {
        if (i == 0) printf("     %d. Memory                                   ", i);
        else if (i == 1) printf("     %d. Memory Performance                       ", i);
        else if (i == 2) printf("     %d. Memory BandWidth                         ", i);
        else if (i == 3) printf("     %d. Memory R/W                               ", i);
        color = getColor(funcCheck[i]);
        result = getResult(funcCheck[i]);
        printf("%s%s%s\n\n", color, result, RESET);
    }

    printf("################### Finish Iteration ##################\n");
}

void cpuTest()
{
    printf("CPU, Processor Check\n");
    cpuNumCheck();

    printf("CPU Performance Check\n");
    cpuPerformCheck();

    printf("CPU IPS Check\n");
    cpuIPSCheck();

    printf("CPU Floating Point Check\n");
    cpuFPCheck();

    printf("CPU Clear\n");

    printSummary();

    return;
}

void memoryTest()
{
    for (int i = 0; i < 5; i++) funcCheck[i] = 0;
    printf("Memory Function Check\n");
    memoryFuncCheck();

    printf("Memory BandWidth Check\n");
    memoryBandWidthCheck();

    printf("Memory Error Check\n");
    memoryErrorCheck();

    printf("Memory Clear\n");

    printSummary();
    return;
} 
