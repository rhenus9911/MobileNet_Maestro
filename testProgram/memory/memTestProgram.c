#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdint.h>
#include <time.h>
#include <math.h>

#define BUFFER_SIZE 1024
#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define RESET "\033[0m"

void printFile(FILE *file) {
    char buffer[BUFFER_SIZE];
    while (fgets(buffer, sizeof(buffer), file)) {
        printf("%s", buffer);
    }
}

void memoryFuncCheck()
{
        char buffer[1024];
        char *ptr;
        double oper;
        double trans;

        FILE *fp = popen("sysbench memory --threads=4 run", "r");
        while(fgets(buffer, sizeof(buffer), fp) != NULL)
        {
                if((ptr = strstr(buffer, "Total operations")) != NULL)
                {
                        sscanf(ptr, "Total operations: %lf", &oper);
                        printf(GREEN "    [+] Total operations: %.2lf\n" RESET, oper);
                }
                else if((ptr = strstr(buffer, "transferred")) != NULL)
                {
                        sscanf(ptr, "transferred (%lf MiB/sec)", &trans);
                        printf(GREEN "    [+] MiB Transferred: %.2lf MiB/sec\n" RESET, trans);
                }
        }

        pclose(fp);
}

int main(int argc, char **argv)
{
        printf("[+] Memory Function Check\n");
        memoryFuncCheck();

        printf("[+] Memory Clear\n");
        return 0;

}
