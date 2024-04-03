#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define _GNU_SOURCE

int main(int argc, char **argv)
{
	
	FILE *cpuinfo;
    char buff[1024];
    memset(buff,NULL,sizeof(buff));

	cpuinfo = popen("grep processor | /proc/cpuinfo | wc -l", "r");
	printf("%x\n",cpuinfo);
	
	if(cpuinfo==NULL){
		printf("cpu is borken\n");
	}
	else if(fgets(buff,sizeof(buff),cpuinfo)!=NULL)
    {
        int nproc = atoi(buff);
        printf("number of processor : %d\n",nproc);
    }
	pclose(cpuinfo);
	return 0;
}

