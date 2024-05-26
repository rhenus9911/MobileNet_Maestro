#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#define PRIMEMAX 1000000
#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define RESET "\033[0m"

bool isPrime(int num)
{
    for (int i = 2;i <= num / 2;i++)
    {
        if (num % i == 0) return 0;
    }
    return 1;
}

void findPrime()
{
    for (int i = 2;i <= PRIMEMAX;i++)
    {
	    if(i == 2) continue;
    	    if (i%2 != 0 && isPrime(i))
		    continue;
    }
}

int main()
{
    clock_t start, end;
    double cpu_time;
    start = clock();
    findPrime();
    end = clock();
    cpu_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf(GREEN "[+] Time used: %.2lf sec\n" RESET, cpu_time);

    return 0;
}
