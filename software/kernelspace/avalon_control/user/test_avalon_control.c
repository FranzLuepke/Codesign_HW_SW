/**
 * @file   test_avalon_control.c
 * @author Franz Luepke
 * @date   8 March 2021
 * @version 0.1
 * @brief  A Linux user space program that communicates with the avalon_control.c LKM. It passes a
 * int to the LKM and the PWM change with this number. For this example to work the device
 * must be called /dev/avalon_control_x.
*/
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>
// DEFINITIONS
#define DEVICES 6			// Number of devices of the same type
#define BUFFER_LENGTH 256	// The buffer length (crude but fine)
#define LOOP_DELAY 10000	// Delay in us
// STATIC CHARS
static char receive[BUFFER_LENGTH];	// The receive buffer from the LKM
// MAIN FUNCTION
int main()
{
	int fd[DEVICES];
	int ret[DEVICES];
	char buffer[BUFFER_LENGTH];
	const char *path;
	printf(" Starting device test code example...\n");
	// Open devices
	printf("  Open devices...\n");
	for (int i = 0; i < DEVICES; ++i)
	{
		char dev_path[BUFFER_LENGTH] = "/dev/avalon_control_";
		sprintf(buffer, "%d", i);
		strcat(dev_path, buffer);
		// path = dev[i];
		path = dev_path;
		printf("   Opening %s...", dev_path);
		fd[i] = open(path, O_RDWR); // Open the device with read/write access	
		if (fd[i] < 0)
		{
			perror(" Failed to open the device.");
			printf("\n");
			return errno;
		}
		else
		{
			printf(" Successfully opened.\n");
		}
	}
	// Reading device
	printf("  Reading from the device...\n");
	while(1)
	{
		printf("   The received message is: ");
		for (int i = 0; i < DEVICES; ++i)
		{
			ret[i] = read(fd[i], &receive, BUFFER_LENGTH); // Read the response from the LKM
			if (ret[i] < 0)
			{
				perror("   Failed to read the message from the device.");
				return errno;
			}
			printf("[%d] ", receive[0]);
		}
		printf("\r");
		usleep(LOOP_DELAY);
	}
	printf("End of the program\n");
	return 0;
}