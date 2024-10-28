#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <netdb.h>


char* get_ip_address() {
    struct ifaddrs *ifaddr, *ifa;
    char *ip_address = (char *)malloc(NI_MAXHOST * sizeof(char));
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
        printf("Failed to execute ifconfig.\n");
    }
}


void ping_test(const char *ip_address) {
    char command[256];
    snprintf(command, sizeof(command), "ping -c 4 %s", ip_address);
    printf("Pinging %s...\n", ip_address);
    int result = system(command);
    if (result != 0) {
        printf("Ping test failed.\n");
    }
}

void iperf_test(const char *server_ip) {
    char command[256];
    snprintf(command, sizeof(command), "iperf3 -c %s", server_ip);
    printf("Testing network speed with iperf3 to server %s...\n", server_ip);
    int result = system(command);
    if (result != 0) {
        printf("iperf3 test failed.\n");
    }
}

int main() {
    // 1. 네트워크 설정 확인
    check_network_interface();

    // 2. 네트워크 연결 테스트
    const char *external_ip = "8.8.8.8";
    ping_test(external_ip);

    // 3. 라즈베리 파이의 IP 주소 확인 및 네트워크 속도 테스트
    char *raspberry_pi_ip = get_ip_address();
    if (raspberry_pi_ip != NULL) {
        iperf_test(raspberry_pi_ip);
        free(raspberry_pi_ip);
    } else {
        printf("Failed to retrieve Raspberry Pi IP address.\n");
    }

    return 0;
}
