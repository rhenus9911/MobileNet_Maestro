#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

#define GPIO_BASE 0xfe200000
#define GPFSEL0 0x00
#define GPSET0 0x1c
#define GPCLR0 0x28

int main(int argc, char **argv)
{
    int fd;
    if((fd = open("/dev/mem", O_RDWR | O_SYNC)) < 0) // 메모리 접근
    {
        perror("[Error] open() : ");
        exit(-1);
    }

    //메모리장치파일에 가상메모리를 할당하는 작업
    //mmap은 가상메모리주소를 반환, 유저영역에선 물리주소를 알아도 가상주소로 변환
    char* gpio_base_addr = (char*)mmap(0, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, GPIO_BASE);
    if(gpio_base_addr == MAP_FAILED)
    {
        perror("[Error] mmap() : ");
        exit(-1);
    }

    volatile unsigned int *gpio_virtual_addr;
    volatile unsigned int *gpfsel0;
    volatile unsigned int *gpset0;
    volatile unsigned int *gpclr0;
    
    gpio_virtual_addr = (volatile unsigned int*)gpio_base_addr;
    gpfsel0 = gpio_virtual_addr + (GPFSEL0/4); //오프셋을 더해줄 떄 메모리는 4bit이기 때문에 나누기 4
    gpset0 = gpio_virtual_addr + (GPSET0/4);
    gpclr0 = gpio_virtual_addr + (GPCLR0/4);
    *gpfsel0 |= (0<<14); // |= or연산 후 값을 넣음 << 왼쪽 시프트
    *gpfsel0 |= (0<<13);
    *gpfsel0 |= (1<<12);

    for(int i=0; i<3; i++)
    {
        *gpset0 |= (1<<4);
        sleep(1);

        *gpclr0 |= (1<<4);
        sleep(1);
    }

    printf("gpio base virtual addr : %p\n", gpio_virtual_addr);
    printf("gpfsel0 virtual addr : %p\n", gpfsel0);
    printf("gpset0 virtual addr : %p\n", gpset0);
    printf("gpclr0 virtual addr : %p\n", gpclr0);

    munmap(gpio_base_addr, 4096);
    close(fd);
    return 0;
}
