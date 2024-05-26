#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define MEM_SIZE 1024

void pattern_test(uint8_t *mem)
{
    size_t i;
    uint8_t p = 0xAA;
    
    for(int i=0;i<MEM_SIZE;i++)
    {
        mem[i] = p;
    }

    for(int i=0;i<MEM_SIZE;i++)
    {
        if(mem[i] != p)
        {
            printf("Memory error at index %zu: expected 0x%02X, got 0x%02X\n", i, p, mem[i]);
            exit(-1);
        }
    }
}

int main()
{
    uint8_t *memory = (uint8_t *)malloc(MEM_SIZE * sizeof(uint8_t));
    if(!memory)
    {
        printf("Memory allocation failed\n");
        exit(-1);   
    }

    pattern_test(memory);
    printf("[+] Memory pattern Clear\n");

    return 0;
}
