//
//  test.c
//  test22
//
//  Created by 김남호 on 2024/04/15.
//

#include "test.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

#define BUFFER_SIZE 1024
#define GPIO_BASE 0xfe200000
#define GPFSEL0 0x00
#define GPSET0 0x1c
#define GPCLR0 0x28
#define GPLEV0 0x34
#define GPFSEL1 0x04
#define GPFSEL2 0x08
#define GPFSEL3 0x0c
#define GPSET1 0x20
#define GPCLR1 0x2c
#define GPLEV1 0x38

void Gpio_Test(void){
    printf("************HardWare Test Start************\n");
    int fd;
        if((fd = open("/dev/mem", O_RDWR | O_SYNC)) < 0)
        {
            perror("[Error] open() : ");
            exit(-1);
        }

        //메모리장치파일에 가상메모리를 할당하는 작업
        //mmap은 가상메모리주소를 반환, 유저영역에선 물리주소를 알아도 가상주소로 변환해야지만
        char* gpio_base_addr = (char*)mmap(0, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, GPIO_BASE);
        if(gpio_base_addr == MAP_FAILED)
        {
            perror("[Error] mmap() : ");
            exit(-1);
        }
        printf("GPIO Pin Test Start : ");
        volatile unsigned int *gpio_virtual_addr;
        volatile unsigned int *gpfsel0; // input output
        volatile unsigned int *gpfsel1; // input output
        volatile unsigned int *gpfsel2; // input output
        //volatile unsigned int *gpfsel3; // input output
        volatile unsigned int *gpset0;
        //volatile unsigned int *gpset1;
        volatile unsigned int *gpclr0;
        //volatile unsigned int *gpclr1;
        volatile unsigned int *gplev0;
        //volatile unsigned int *gplev1;
        gpio_virtual_addr = (volatile unsigned int*)gpio_base_addr;
        gpfsel0 = gpio_virtual_addr + (GPFSEL0/4);
        gpfsel1 = gpio_virtual_addr + (GPFSEL1/4);
        gpfsel2 = gpio_virtual_addr + (GPFSEL2/4);
        //gpfsel3 = gpio_virtual_addr + (GPFSEL3/4);
        gpset0 = gpio_virtual_addr + (GPSET0/4);
        //gpset1 = gpio_virtual_addr + (GPSET1/4);
        gpclr0 = gpio_virtual_addr + (GPCLR0/4);
        //gpclr1 = gpio_virtual_addr + (GPCLR1/4);
        gplev0 = gpio_virtual_addr + (GPLEV0/4);
        //gplev1 = gpio_virtual_addr + (GPLEV1/4);
        
        //int k = 9;
        //*gpfsel0 |= (0<<11);
        //*gpfsel0 |= (0<<10);
        //*gpfsel0 |= (1<<k); // 21 23
        //*gpfsel1 |= (0<<11);
        //*gpfsel1 |= (0<<10);
        //*gpfsel1 |= (1<<9);
        //*gpfsel3 |= (0<<14);
        //*gpfsel3 |= (0<<13);
        //*gpfsel3 |= (1<<12);
        //*gpclr0 |= (1 << 3);
        unsigned int pin;
        //printf("%d", pin);
        int k = 0;
        int check = 0;
        for(int i=0; i<1; i++){
            for(int j = 0; j < 27; j++){
                if(j < 10){
                    *gpfsel0 |= (1<<k);
                    *gpset0 |= (1 << j);
                    pin = (*gplev0 >> j) & 1;
                    if(!pin) {
                        printf("%d, ", j);
                        check = 1;
                        }
                    sleep(1);
                    *gpclr0 |= (1 << j);
                    pin = (*gplev0 >> j) & 1;
                    if(pin) {
                        printf("%d, ", j);
                        check = 1;
                        }
                    sleep(1);
                    *gpfsel0 &= 0;
                    *gpset0 &= 0;
                    *gpclr0 &= 0;
                    k += 3;
                }
                else if(j < 20){
                    if(j == 10) k = 0;
                    *gpfsel1 |= (1<<k);
                    *gpset0 |= (1 << j);
                    pin = (*gplev0 >> j) & 1;
                    if(!pin) {
                        printf("%d, ", j);
                        check = 1;
                        }
                    sleep(1);
                    *gpclr0 |= (1 << j);
                    pin = (*gplev0 >> j) & 1;
                    if(pin) {
                        printf("%d, ", j);
                        check = 1;
                        }
                    sleep(1);
                    *gpfsel1 &= 0;
                    *gpset0 &= 0;
                    *gpclr0 &= 0;
                    k += 3;
                }
                else{
                    if(j == 20) k = 0;
                    *gpfsel2 |= (1<<k);
                    *gpset0 |= (1 << j);
                    pin = (*gplev0 >> j) & 1;
                    if(!pin) {
                        printf("%d, ", j);
                        check = 1;
                        }
                    sleep(1);
                    *gpclr0 |= (1 << j);
                    pin = (*gplev0 >> j) & 1;
                    if(pin) {
                        printf("%d, ", j);
                        check = 1;
                        }
                    sleep(1);
                    *gpfsel2 &= 0;
                    *gpset0 &= 0;
                    *gpclr0 &= 0;
                    //if(j == 25) k -= 6; // failed test
                    k += 3;
                }
            }
            
        }
        if(check) printf("fail \n");
        else printf("sucess \n");
        //printf("gpio base virtual addr : %p\n", gpio_virtual_addr);
        //printf("gpfsel0 virtual addr : %p\n", gpfsel0);
        //printf("gpset0 virtual addr : %p\n", gpset0);
        //printf("gpclr0 virtual addr : %p\n", gpclr0);
        printf("GPIO Finsih \n");
        munmap(gpio_base_addr, 4096);
        close(fd);
        
}
void Cpu_Test(void){
        FILE *cpuinfo = popen("grep processor /proc/cpuinfo | wc -l", "r");
        FILE *modelname = popen("grep Model /proc/cpuinfo", "r");
        FILE *lscpu = popen("lscpu | grep 'CPU'", "r");
        FILE *lsmem = popen("free | grep Mem | awk '{print $7}'", "r");
        
        if (cpuinfo == NULL || modelname == NULL || lscpu == NULL || lsmem == NULL) {
            printf("Failed to run command\n");
            return;
        }

        //printFile(lscpu);
        printf("CPU Test Start \n");
        char buffer[BUFFER_SIZE];
        if (fgets(buffer, sizeof(buffer), lsmem) != NULL) {
            int mem_avail = atoi(buffer);
            if (mem_avail > 0) {
                printf("Memory access Okay\n");
            }
        }

        //printFile(modelname);

        if (fgets(buffer, sizeof(buffer), cpuinfo) != NULL) {
            int num_processors = atoi(buffer);
            printf("Number of processors: %d\n", num_processors);
            if (num_processors == 4) {
                printf("Processor is alright\n");
            }
        } else {
            printf("Failed to read output\n");
            return;
        }
        printf("CPU Test Finsih \n");
        pclose(cpuinfo);
        pclose(modelname);
        pclose(lscpu);
        pclose(lsmem);

    
}


// int main(){
//     Gpio_Test();
//     Cpu_Test();

// }
