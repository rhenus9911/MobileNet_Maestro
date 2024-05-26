#include <stdio.h>
#include <time.h>

int main() {
    clock_t start, end;
    double cpu_time_used;
    long long int instructions = 10006964235;
    int loop = 1000000000; // 10억 번 반복
    double result = 0.0;

    start = clock();
    for (int i = 0; i < loop; i++) {}
    end = clock();

    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Time used: %f seconds\n", cpu_time_used);

    result = instructions / cpu_time_used;
    result /= (1000 * 1000 * 1000);
    printf("Instructions Per Second: %.2lf GIPS\n", result);

    return 0;
}
