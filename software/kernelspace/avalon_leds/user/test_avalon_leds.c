/**
 * @file	test_avalon_leds.c
 * @author	Franz Luepke
 * @date	8 March 2021
 * @version	0.1
 * @brief	A Linux user space program that communicates with the avalon_leds.c LKM. It passes a
 * int to the LKM and the LEDs change with this number. For this example to work the device
 * must be called /dev/avalon_leds.
*/
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>
#define BUFFER_LENGTH 8               ///< The buffer length (crude but fine)
// Constants
const char *pathname = "/dev/avalon_leds";
static char receive[BUFFER_LENGTH];     ///< The receive buffer from the LKM
int main()
{
	int ret;
	int fd;
	int intToSend;
	char operation[1];
	char stringToSend[BUFFER_LENGTH];
	printf("Starting device test code example...\n");
	fd = open(pathname, O_RDWR); // Open the device with read/write access
	if (fd < 0)
	{
		perror("Failed to open the device...");
		return errno;
	}
	while(1)
	{
		printf(" Read or write from module? (r/w) [q to quit] ");
		scanf("%s", operation);   // Read operation
		if(strcmp(operation, "w")==0)
		{
			printf("  Type in a number to send to the kernel module: ");
			scanf("%d", &intToSend);   // Read in an int
			stringToSend[0] = (int) intToSend;
			printf("   Writing message to the device: [%d].\n", stringToSend[0]);
			ret = write(fd, &stringToSend[0], sizeof(stringToSend)); // Send the string to the LKM
			if (ret < 0)
			{
				perror("   Failed to write the message to the device.");
				return errno;
			}
		}
		else if(strcmp(operation, "r")==0)
		{
			ret = read(fd, receive, BUFFER_LENGTH); // Read the response from the LKM
			if (ret < 0)
			{
				perror("  Failed to read the message from the device.");
				return errno;
			}
			printf("  The received message is: [%d]\n", (int) receive[0]);
		}
		else if(strcmp(operation, "q")==0)
		{
			break;
		}
		else
		{
			printf("  Not recognized.\n");
		}
	}
	printf("End of the program\n");
	return 0;
}