#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdint.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>

#define BUFFER_SIZE 1024
#define STREAM_ARRAY_SIZE 100000000
#define NTIMES 20

#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define YELLOW "\033[0;33m"
#define RESET "\033[0m"

static double a[STREAM_ARRAY_SIZE];
static double b[STREAM_ARRAY_SIZE];

int funcCheck[4] = {0};

double timeCheck()
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double) ts.tv_sec + (double) ts.tv_nsec * 1.e-6;
}

void memoryFuncCheck()
{
    char buffer[1024];
    char *ptr;
    double oper;
    double trans;

    FILE *lsmem = popen("free | grep Mem | awk '{print $7}'", "r");
    FILE *fp = popen("sysbench memory --threads=4 run", "r");
    if(lsmem == NULL)
    {
        printf(RED "    [!] Memory is not Access\n" RESET);
        exit(-1);
    }
    else
    {
        printf("    [+] Memory is Access\n");
        funcCheck[0] = 2;
    }
    while(fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        if((ptr = strstr(buffer, "Total operations")) != NULL)
        {
            sscanf(ptr, "Total operations: %lf", &oper);
            printf("    [+] Total operations: %.2lf\n", oper);
        }
        else if((ptr = strstr(buffer, "transferred")) != NULL)
        {
            sscanf(ptr, "transferred (%lf MiB/sec)", &trans);
            printf("    [+] MiB Transferred: %.2lf MiB/sec\n", trans);
        }
    }

    if(oper >= 20000000.0) funcCheck[1] = 2;
    else if(oper >= 5000000) funcCheck[1] = 1;
    else funcCheck[1] = 0;

    pclose(fp);
    pclose(lsmem);
}

void readBandWidth(double *array)
{
    volatile double value;
    for(size_t i=0;i<STREAM_ARRAY_SIZE;i++)
    {
        value = array[i];
    }
}

void writeBandWidth(double *array)
{
    for(size_t i=0;i<STREAM_ARRAY_SIZE;i++)
    {
        array[i] = i;          
    }
}

void copyBandWidth()
{
    for(size_t i=0;i<STREAM_ARRAY_SIZE;i++)
    {
        b[i] = a[i];
    }
}

char* getColor(int check)
{
    if(check == 0) return RED;
    else if(check == 1) return YELLOW;
    else return GREEN;
}

char* getResult(int check)
{
    if(check == 0) return "FAIL";
    else if(check == 1) return "WARN";
    else return "PASS";
}

void printSummary()
{
    char *color;
    char *result;

    printf("=======================================================\n\n");
    printf(" Index  Test discription                        Result \n\n");
    printf("=======================================================\n\n");

    for(int i=0;i<4;i++)
    {
        if(i == 0) printf("     %d. Memory                                   ", i);
        else if(i == 1) printf("     %d. Memory Performance                       ", i);
        else if(i == 2) printf("     %d. Memory BandWidth                         ", i);
        else if(i == 3) printf("     %d. Memory R/W                               ", i);
        color = getColor(funcCheck[i]);
        result = getResult(funcCheck[i]);
        printf("%s%s%s\n\n", color, result, RESET);
    }

    printf("################### Finish Iteration ##################\n");
}

void memoryBandWidthCheck()
{
    double times[3][NTIMES];
    double avg_t[3], min_t[3], max_t[3], best_rate[3];
    double *array = (double *)malloc(STREAM_ARRAY_SIZE * sizeof(double));
    
    for(int k=0; k<STREAM_ARRAY_SIZE; k++)
    {
        
        b[k] = 0.0;
        array[k] = 0.0;
    }

    for(int k=0;k<NTIMES;k++)
    {
        double start = timeCheck();
        readBandWidth(array);
        double end = timeCheck();
        times[0][k] = end - start;
    }

    for(int k=0;k<NTIMES;k++)
    {
        double start = timeCheck();
        writeBandWidth(array);
        double end = timeCheck();
        times[1][k] = end - start;
    }

    for(int k=0;k<NTIMES;k++)
    {
        double start = timeCheck();
        copyBandWidth(a, b);
        double end = timeCheck();
        times[2][k] = end - start;
    }

    for(int i=0;i<3;i++)
    {
        min_t[i] = times[i][0];
        max_t[i] = times[i][0];
        avg_t[i] = 0.0;
        for(int k=0;k<NTIMES;k++)
        {
            if(min_t[i] > times[i][k]) min_t[i] = times[i][k];
            if(max_t[i] < times[i][k]) max_t[i] = times[i][k];
            avg_t[i] += times[i][k];
        }

        avg_t[i] /= NTIMES;

        best_rate[i] = (2.0 * STREAM_ARRAY_SIZE * sizeof(double)) / (min_t[i] * 1.0e6);
    }
    printf("    Function    Best Rate MB/s  Avg time    Min time    Max time\n");
    printf("    Read:   %11.2lf         %8.6lf    %8.6lf    %8.6lf\n", best_rate[0], avg_t[0], min_t[0], max_t[0]);
    printf("    Write:  %11.2lf         %8.6lf    %8.6lf    %8.6lf\n", best_rate[1], avg_t[1], min_t[1], max_t[1]);
    printf("    Copy:   %11.2lf         %8.6lf    %8.6lf    %8.6lf\n", best_rate[2], avg_t[2], min_t[2], max_t[2]);

    if(avg_t[0] >= 4000 && avg_t[1] >= 1600 && avg_t[2] >= 1500) funcCheck[2] = 2;
    else if(avg_t[0] < 4000 && avg_t[1] < 1600 && avg_t[2] < 1500) funcCheck[2] = 0;
    else funcCheck[2] = 1;
    
    free(array);
}

void memoryErrorCheck()
{
    uint8_t p = 0xAA;
    uint8_t *memory = (uint8_t *)malloc(BUFFER_SIZE * sizeof(uint8_t));
    if(!memory)
    {
        printf("    [!] Memory allocation failed\n");
        exit(-1);   
    }

    for(size_t i=0;i<BUFFER_SIZE;i++)
    {
        memory[i] = p;
    }

    for(size_t i=0;i<BUFFER_SIZE;i++)
    {
        if(memory[i] != p)
        {
            printf("    [!] Memory error at index %zu: expected 0x%02X, got 0x%02X\n", i, p, memory[i]);
            funcCheck[3] = 0;
            exit(-1);
        }
    }

    funcCheck[3] = 2;
}

int main(int argc, char **argv)
{
    printf("[+] Memory Function Check\n");
    //memoryFuncCheck();

    printf("[+] Memory BandWidth Check\n");
    memoryBandWidthCheck();

    printf("[+] Memory Error Check\n");
    //memoryErrorCheck();

    printf("[+] Memory Clear\n");

    //printSummary();
    return 0;
}

