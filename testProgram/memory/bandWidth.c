#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>

#define STREAM_ARRAY_SIZE 100000000
#define NTIMES 20

double *array;
static double a[STREAM_ARRAY_SIZE];
static double b[STREAM_ARRAY_SIZE];

double timeCheck()
{
    struct timeval tp;
    gettimeofday(&tp, NULL);
    return (double) tp.tv_sec + (double) tp.tv_usec * 1.e-6;
}

void copyBandWidth()
{
    for(size_t i=0;i<STREAM_ARRAY_SIZE;i++)
        b[i] = a[i];
}

void readBandWidth()
{
    volatile double value;
    for(size_t i=0;i<STREAM_ARRAY_SIZE;i++)
        value = array[i];
}

void writeBandWidth()
{
    for(size_t i=0;i<STREAM_ARRAY_SIZE;i++)
        array[i] = i;
}

int main()
{
    double times[3][NTIMES];
    double avg_t[3], min_t[3], max_t[3], best_rate[3];
    array = (double *)malloc(STREAM_ARRAY_SIZE * sizeof(double));
    printf("[+]Memory BandWitch Check\n");

    for(int k=0;k<STREAM_ARRAY_SIZE;k++)
    {
        a[k] = 1.0;
        b[k] = 0.0;
        array[k] = 0.0;
    }

    for(int k=0;k<NTIMES;k++)
    {
        double start = timeCheck();
        copyBandWidth();
        double end = timeCheck();
        times[0][k] = end-start;
    }

    for(int k=0;k<NTIMES;k++)
    {
        double start = timeCheck();
        readBandWidth();
        double end = timeCheck();
        times[1][k] = end-start;
    }

    for(int k=0;k<NTIMES;k++)
    {
        double start = timeCheck();
        writeBandWidth();
        double end = timeCheck();
        times[2][k] = end-start;
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
    printf("Function    Best Rate MB/s  Avg time    Min time    Max time\n");
    printf("Copy:   %11.2lf         %8.6lf    %8.6lf    %8.6lf\n", best_rate[0], avg_t[0], min_t[0], max_t[0]);
    printf("Read:   %11.2lf         %8.6lf    %8.6lf    %8.6lf\n", best_rate[1], avg_t[1], min_t[1], max_t[1]);
    printf("Write:  %11.2lf         %8.6lf    %8.6lf    %8.6lf\n", best_rate[2], avg_t[2], min_t[2], max_t[2]);

    return 0;
}
