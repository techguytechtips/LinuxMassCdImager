#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/cdrom.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#define RAM 10485760
int main (int argc, char* argv[])
{
	if (argc < 2){
		printf("Error: Not enough args.\nUsage: %s <cdrom>\n", argv[0]);
		return -1;
	}
	char name[30];
	int readstatus;
	int writestatus;
	unsigned long total;
	int fd; 
	char *buffer = (char *)malloc(RAM);
	unsigned long size;
	fd = open(argv[1], O_RDONLY);
	if(fd < 0)
	{
		printf("Error: failed to read disc from %s. Please make sure the path is correct and that there is a disc in the drive\n", argv[1]);
		return -1;
	
	}
	
	while(1)
	{
		

		total = 0, readstatus = 0, writestatus = 0;
		if(ioctl(fd, CDROM_DRIVE_STATUS) == 4){
			printf("DRIVE READY\n");
			printf("name of ISO: ");		
			scanf("%s", &name);	
			strcat(name, ".iso");
		        int wfd = open(name,O_RDWR | O_TRUNC | O_CREAT | O_APPEND, 774);
			printf("errno: %d\n", errno);
			if (fd < 0 || wfd < 0)
			{
				printf("%d, %d, Error: failed to open file or drive!", fd, wfd);

			}	
			size = lseek(fd, 0, SEEK_END);
			lseek(fd, 0, SEEK_SET);	
			printf("%lu\n", size);	

			do{
			readstatus = read(fd, buffer, RAM);
			writestatus = write(wfd, buffer, RAM);
			if (readstatus < 1 || writestatus < 1)
			{
				printf("I/O Error\n");
	
			}
			else
				total += writestatus;
			printf("total: %lu\n", total);
			}while(total < size);
    		
			close(wfd);


		
			while(ioctl(fd, CDROM_DRIVE_STATUS) != 2){
				ioctl(fd, CDROMEJECT);
				sleep(2);			
			}
			
		}
		else
			sleep(5);	
	}
	close(fd);
	return 0;
}

