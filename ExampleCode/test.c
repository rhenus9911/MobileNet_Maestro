#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(int argc, char **argv)
{
    FILE *cpuinfo;
    char buffer[1024]; // 버퍼 크기는 적당히 조정해야 합니다.

    // 쉘을 통해 명령어 실행
    cpuinfo = popen("grep processor /proc/cpuinfo | wc -l", "r");

    // popen이 제대로 실행되었는지 확인
    if(cpuinfo == NULL){
        printf("Failed to run command\n");
        return 1;
    }

    // 명령어의 출력을 읽어옴
    if(fgets(buffer, sizeof(buffer), cpuinfo) != NULL){
        int num_processors = atoi(buffer);
        printf("Number of processors: %d\n", num_processors);
    }
    else {
        printf("Failed to read output\n");
        return 1;
    }

    // popen 종료
    pclose(cpuinfo);

    return 0;
}
