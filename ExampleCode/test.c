#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(int argc, char **argv)
{
    FILE *cpuinfo;
    FILE *modelname;
    FILE *lscpu;

    char buffer[1024]; 

    cpuinfo = popen("grep processor /proc/cpuinfo | wc -l", "r");
    modelname = popen("grep Model /proc/cpuinfo","r");
    lscpu = popen("lscpu | grep 'CPU'", "r");


    if(cpuinfo == NULL){
        printf("Failed to run command\n");
        return 1;
    }

    while(fgets(buffer,sizeof(buffer),lscpu)){
        printf("%s",buffer);
    }

    if(fgets(buffer, sizeof(buffer), modelname) != NULL){
        printf("%s",buffer);
    }

    if(fgets(buffer, sizeof(buffer), cpuinfo) != NULL){
        int num_processors = atoi(buffer);
        printf("Number of processors: %d\n", num_processors);
        if(num_processors==4){
            printf("Proccesor is alright\n");
        }
    }
    else {
        printf("Failed to read output\n");
        return 1;
    }

    pclose(cpuinfo);
    pclose(modelname);
    pclose(lscpu);

    return 0;
}
