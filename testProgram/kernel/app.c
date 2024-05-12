#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{

	printf("OPEN FILE\n");
	int fd = open("/dev/nobrand", O_RDONLY);
	if(fd == -1)
	{
		printf("FILE OPEN ERROR\n");
		return 0;
	}

	ioctl(fd, 3, 0);
	ioctl(fd, 4, 0);
	
	// write
	write(fd, "Let's go\n", 7);
	
	// read
	char buf[100];
	read(fd, buf, sizeof(buf)-1);
	printf("%s\n", buf);
	printf("%s\n", buf);

	printf("CLOSE FILE!!\n");
	close(fd);

	return 0;
}
