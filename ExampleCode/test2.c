#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024

void printFile(FILE *file) {
    char buffer[BUFFER_SIZE];
    while (fgets(buffer, sizeof(buffer), file)) {
        printf("%s", buffer);
    }
}

int main(int argc, char **argv) {
    FILE *cpuinfo = popen("grep processor /proc/cpuinfo | wc -l", "r");
    FILE *modelname = popen("grep Model /proc/cpuinfo", "r");
    FILE *lscpu = popen("lscpu | grep 'CPU'", "r");
    FILE *lsmem = popen("free | grep Mem | awk '{print $7}'", "r");

    if (cpuinfo == NULL || modelname == NULL || lscpu == NULL || lsmem == NULL) {
        printf("Failed to run command\n");
        return 1;
    }

    printFile(lscpu);

    char buffer[BUFFER_SIZE];
    if (fgets(buffer, sizeof(buffer), lsmem) != NULL) {
        int mem_avail = atoi(buffer);
        if (mem_avail > 0) {
            printf("Memory access Okay\n");
        }
    }

    printFile(modelname);

    if (fgets(buffer, sizeof(buffer), cpuinfo) != NULL) {
        int num_processors = atoi(buffer);
        printf("Number of processors: %d\n", num_processors);
        if (num_processors == 4) {
            printf("Processor is alright\n");
        }
    } else {
        printf("Failed to read output\n");
        return 1;
    }

    pclose(cpuinfo);
    pclose(modelname);
    pclose(lscpu);
    pclose(lsmem);

    return 0;
}
