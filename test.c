#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>
#include <wiringPiSPI.h>
#include <wiringPi.h>
#include <sys/types.h>
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <bluetooth/rfcomm.h>
#include <sys/socket.h>
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
#define SPI_CHANNEL_0 0  // SPI 채널 0 (CE0)
#define SPI_CHANNEL_1 1  // SPI 채널 1 (CE1)
#define SPI_SPEED 500000 // SPI 속도 (500kHz)
#define DATA_LENGTH 10
#define BUFFER_SIZE 1024


int GpioTest() {
    int fd;
    if ((fd = open("/dev/mem", O_RDWR | O_SYNC)) < 0)
    {
        perror("[Error] open() : ");
        exit(-1);
    }

    //메모리장치파일에 가상메모리를 할당하는 작업
    //mmap은 가상메모리주소를 반환, 유저영역에선 물리주소를 알아도 가상주소로 변환해야지만 
    char* gpio_base_addr = (char*)mmap(0, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, GPIO_BASE);
    if (gpio_base_addr == MAP_FAILED)
    {
        perror("[Error] mmap() : ");
        exit(-1);
    }

    volatile unsigned int* gpio_virtual_addr;
    volatile unsigned int* gpfsel0; // input output
    volatile unsigned int* gpfsel1; // input output
    volatile unsigned int* gpfsel2; // input output
    //volatile unsigned int *gpfsel3; // input output
    volatile unsigned int* gpset0;
    //volatile unsigned int *gpset1;
    volatile unsigned int* gpclr0;
    //volatile unsigned int *gpclr1;
    volatile unsigned int* gplev0;
    //volatile unsigned int *gplev1;
    gpio_virtual_addr = (volatile unsigned int*)gpio_base_addr;
    gpfsel0 = gpio_virtual_addr + (GPFSEL0 / 4);
    gpfsel1 = gpio_virtual_addr + (GPFSEL1 / 4);
    gpfsel2 = gpio_virtual_addr + (GPFSEL2 / 4);
    //gpfsel3 = gpio_virtual_addr + (GPFSEL3/4);
    gpset0 = gpio_virtual_addr + (GPSET0 / 4);
    //gpset1 = gpio_virtual_addr + (GPSET1/4);
    gpclr0 = gpio_virtual_addr + (GPCLR0 / 4);
    //gpclr1 = gpio_virtual_addr + (GPCLR1/4);
    gplev0 = gpio_virtual_addr + (GPLEV0 / 4);
    //gplev1 = gpio_virtual_addr + (GPLEV1/4);

    //int k = 9;
    //*gpfsel0 |= (0<<11);
    //*gpfsel0 |= (0<<10);
    //*gpfsel0 |= (1<<k); // 21 23
    //*gpfsel1 |= (0<<11);
    //*gpfsel1 |= (0<<10);
    //*gpfsel1 |= (1<<9);
    //*gpfsel3 |= (0<<14);
    //*gpfsel3 |= (0<<13);
    //*gpfsel3 |= (1<<12);
    //*gpclr0 |= (1 << 3);
    unsigned int pin;
    //printf("%d", pin);
    int k = 0;
    int check = 0;
    for (int i = 0; i < 1; i++) {
        for (int j = 0; j < 27; j++) {
            if (j < 10) {
                *gpfsel0 |= (1 << k);
                *gpset0 |= (1 << j);
                pin = (*gplev0 >> j) & 1;
                if (!pin) {
                    printf("%d, ", j);
                    check = 1;
                }
                sleep(1);
                *gpclr0 |= (1 << j);
                pin = (*gplev0 >> j) & 1;
                if (pin) {
                    printf("%d, ", j);
                    check = 1;
                }
                sleep(1);
                *gpfsel0 &= 0;
                *gpset0 &= 0;
                *gpclr0 &= 0;
                k += 3;
            }
            else if (j < 20) {
                if (j == 10) k = 0;
                *gpfsel1 |= (1 << k);
                *gpset0 |= (1 << j);
                pin = (*gplev0 >> j) & 1;
                if (!pin) {
                    printf("%d, ", j);
                    check = 1;
                }
                sleep(1);
                *gpclr0 |= (1 << j);
                pin = (*gplev0 >> j) & 1;
                if (pin) {
                    printf("%d, ", j);
                    check = 1;
                }
                sleep(1);
                *gpfsel1 &= 0;
                *gpset0 &= 0;
                *gpclr0 &= 0;
                k += 3;
            }
            else {
                if (j == 20) k = 0;
                *gpfsel2 |= (1 << k);
                *gpset0 |= (1 << j);
                pin = (*gplev0 >> j) & 1;
                if (!pin) {
                    printf("%d, ", j);
                    check = 1;
                }
                sleep(1);
                *gpclr0 |= (1 << j);
                pin = (*gplev0 >> j) & 1;
                if (pin) {
                    printf("%d, ", j);
                    check = 1;
                }
                sleep(1);
                *gpfsel2 &= 0;
                *gpset0 &= 0;
                *gpclr0 &= 0;
                //if(j == 25) k -= 6; // failed test  
                k += 3;
            }
        }

    }
    if (check) {
        printf("\033[31mGpio Failed\033[0m\n");
        return 0;
    }
    else {
        printf("\033[32mGpio Success\033[0m\n");
        return 1;
    }
    //printf("gpio base virtual addr : %p\n", gpio_virtual_addr);
    //printf("gpfsel0 virtual addr : %p\n", gpfsel0);
    //printf("gpset0 virtual addr : %p\n", gpset0);
    //printf("gpclr0 virtual addr : %p\n", gpclr0);

    munmap(gpio_base_addr, 4096);
    close(fd);
	
}

void setup() {
    if (wiringPiSetup() == -1) {
        printf("wiringPi 초기화 실패\n");
        exit(1);
    }

    pinMode(PWM_PIN18, PWM_OUTPUT);
    pinMode(PWM_PIN12, PWM_OUTPUT);
    pinMode(PWM_PIN13, PWM_OUTPUT);
    pinMode(PWM_PIN19, PWM_OUTPUT);
    pinMode(INPUT_PIN, INPUT);

    pwmSetMode(PWM_MODE_MS);
    pwmSetRange(128);
    pwmSetClock(32);
}
int logPWMInput() {
    int value;
    int highCount = 0;
    int lowCount = 0;
    int totalCount = 128; // 측정할 샘플 수

    for (int i = 0; i < totalCount; i++) {
        value = digitalRead(INPUT_PIN);

        if (value == HIGH) {
            highCount++;
        }
        else {
            lowCount++;
        }

        delayMicroseconds(100); // 100us마다 샘플링
    }

    printf("HIGH Count: %d, LOW Count: %d\n", highCount, lowCount);
    printf("HIGH Percentage: %.2f%%, LOW Percentage: %.2f%%\n",
        (highCount / (float)totalCount) * 100,
        (lowCount / (float)totalCount) * 100);
    return highCount / (float)totalCount * 100;
}

int PWMTest() {
    setup();
    int n;
     pwmWrite(PWM_PIN13, 50); // 50% duty cycle
     printf("input value : %d\n", 50);
     n = logPWMInput();
     sleep(1); // 1초마다 로그 출력
     if (n >= 49.5 && n <= 50.5) {
         printf("\033[32mPWM Success\033[0m\n");
         return 1;
    }
     else {
         printf("\033[31mPWM Failed\033[0m\n");
         return 0;
     }
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


    // SPI로 데이터 전송 및 수신
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
    // wiringPi 초기화
    if (wiringPiSetup() == -1) {
        printf("wiringPi setup failed!\n");
        return 1;
    }

    // SPI 채널 초기화
    if (wiringPiSPISetup(SPI_CHANNEL_0, SPI_SPEED) == -1) {
        printf("SPI setup failed on channel 0!\n");
        return 1;
    }

    if (wiringPiSPISetup(SPI_CHANNEL_1, SPI_SPEED) == -1) {
        printf("SPI setup failed on channel 1!\n");
        return 1;
    }
    int n;
    // 채널 0 (CE0) 테스트
    n = spi_loopback_test(SPI_CHANNEL_0);
    return n;

    
}

int SPITest_1() {
    // wiringPi 초기화
    if (wiringPiSetup() == -1) {
        printf("wiringPi setup failed!\n");
        return 1;
    }

    // SPI 채널 초기화
    if (wiringPiSPISetup(SPI_CHANNEL_0, SPI_SPEED) == -1) {
        printf("SPI setup failed on channel 0!\n");
        return 1;
    }

    if (wiringPiSPISetup(SPI_CHANNEL_1, SPI_SPEED) == -1) {
        printf("SPI setup failed on channel 1!\n");
        return 1;
    }

   
    int n;
    // 채널 1 (CE1) 테스트
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

    // 명령 출력을 버퍼에 저장
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        // ESSID 확인
        if (strstr(buffer, "ESSID") != NULL) {
            if (strstr(buffer, "off/any") == NULL) {
                is_connected = 1;
                printf("\033[32mConnected to Wi-Fi network.\033[0m\n");
                printf("%s", buffer);
                break;
            }
        }
    }

    // 연결된 경우 추가 정보 출력
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
    // 1. 네트워크 설정 확인
    check_network_interface();

    // 2. 네트워크 연결 테스트
    const char* external_ip = "8.8.8.8";
    ping_test(external_ip);

    // 3. 라즈베리 파이의 IP 주소 확인 및 네트워크 속도 테스트
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
        printf("No connected devices.\n");
        return 0;
    }
    else return 1;

    free(ii);
    close(sock);
}
