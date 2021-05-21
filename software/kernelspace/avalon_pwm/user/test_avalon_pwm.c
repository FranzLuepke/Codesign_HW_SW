/**
 * @file   test_avalon_pwm.c
 * @author Franz Luepke
 * @date   8 March 2021
 * @version 0.1
 * @brief  A Linux user space program that communicates with the avalon_pwm.c LKM. It passes a
 * int to the LKM and the PWM change with this number. For this example to work the device
 * must be called /dev/avalon_pwm.
*/
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>
#define BUFFER_LENGTH 256				///< The buffer length (crude but fine)
static char receive_0[BUFFER_LENGTH];	///< The receive buffer from the LKM
static char receive_1[BUFFER_LENGTH];	///< The receive buffer from the LKM
static char receive_2[BUFFER_LENGTH];	///< The receive buffer from the LKM
static char receive_3[BUFFER_LENGTH];	///< The receive buffer from the LKM
static char receive_4[BUFFER_LENGTH];	///< The receive buffer from the LKM
static char receive_5[BUFFER_LENGTH];	///< The receive buffer from the LKM
int main()
{
	int ret_0;
	int ret_1;
	int ret_2;
	int ret_3;
	int ret_4;
	int ret_5;
	int fd_0;
	int fd_1;
	int fd_2;
	int fd_3;
	int fd_4;
	int fd_5;
	// char stringToSend[BUFFER_LENGTH];
	printf("Starting device test code example...\n");
	fd_0 = open("/dev/avalon_pwm_0", O_RDWR); // Open the device with read/write access
	if (fd_0 < 0)
	{
		perror("Failed to open the device 0...");
		return errno;
	}
	fd_1 = open("/dev/avalon_pwm_1", O_RDWR); // Open the device with read/write access
	if (fd_1 < 0)
	{
		perror("Failed to open the device 1...");
		return errno;
	}
	fd_2 = open("/dev/avalon_pwm_2", O_RDWR); // Open the device with read/write access
	if (fd_2 < 0)
	{
		perror("Failed to open the device 2...");
		return errno;
	}
	fd_3 = open("/dev/avalon_pwm_3", O_RDWR); // Open the device with read/write access
	if (fd_3 < 0)
	{
		perror("Failed to open the device 3...");
		return errno;
	}
	fd_4 = open("/dev/avalon_pwm_4", O_RDWR); // Open the device with read/write access
	if (fd_4 < 0)
	{
		perror("Failed to open the device 4...");
		return errno;
	}
	fd_5 = open("/dev/avalon_pwm_5", O_RDWR); // Open the device with read/write access
	if (fd_5 < 0)
	{
		perror("Failed to open the device 5...");
		return errno;
	}
	// printf("Type in a string to send its hex value to the kernel module:\n");
	// scanf("%[^\n]%*c", stringToSend);   // Read in a string (with spaces)
	// printf("Writing message to the device [%s].\n", stringToSend);
	// ret = write(fd, stringToSend, strlen(stringToSend)); // Send the string to the LKM
	// if (ret < 0)
	// {
	// 	perror("Failed to write the message to the device.");
	// 	return errno;
	// }
	// printf("Press ENTER to read back from the device...\n");
	// getchar();
	printf("Reading from the device...\n");
	while(1)
	{
		ret_0 = read(fd_0, receive_0, BUFFER_LENGTH); // Read the response from the LKM
		if (ret_0 < 0)
		{
			perror("Failed to read the message from the device.");
			return errno;
		}
		ret_1 = read(fd_1, receive_1, BUFFER_LENGTH); // Read the response from the LKM
		if (ret_1 < 0)
		{
			perror("Failed to read the message from the device.");
			return errno;
		}
		ret_2 = read(fd_2, receive_2, BUFFER_LENGTH); // Read the response from the LKM
		if (ret_2 < 0)
		{
			perror("Failed to read the message from the device.");
			return errno;
		}
		ret_3 = read(fd_3, receive_3, BUFFER_LENGTH); // Read the response from the LKM
		if (ret_3 < 0)
		{
			perror("Failed to read the message from the device.");
			return errno;
		}
		ret_4 = read(fd_4, receive_4, BUFFER_LENGTH); // Read the response from the LKM
		if (ret_4 < 0)
		{
			perror("Failed to read the message from the device.");
			return errno;
		}
		ret_5 = read(fd_5, receive_5, BUFFER_LENGTH); // Read the response from the LKM
		if (ret_5 < 0)
		{
			perror("Failed to read the message from the device.");
			return errno;
		}
		printf("\r The received message is: [%s] [%s] [%s] [%s] [%s] [%s]", receive_0, receive_1, receive_2, receive_3, receive_4, receive_5);
		usleep(10000);
	}
	printf("End of the program\n");
	return 0;
}