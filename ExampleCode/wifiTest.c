#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024

void check_wifi_connection() {
    char buffer[BUFFER_SIZE];
    FILE *fp;
    int is_connected = 0;
    
    fp = popen("iwconfig 2>&1", "r");
    if (fp == NULL) {
        printf("Failed to run iwconfig\n");
        return;
    }

    // 명령 출력을 버퍼에 저장
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        // ESSID 확인
        if (strstr(buffer, "ESSID") != NULL) {
            if (strstr(buffer, "off/any") == NULL) {
                is_connected = 1;
                printf("Connected to Wi-Fi network.\n");
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
    } else {
        printf("Not connected to any Wi-Fi network.\n");
    }

    
    pclose(fp);
}

int main() {
    check_wifi_connection();
    return 0;
}
