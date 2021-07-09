/**
 * @file   test_avalon_enable.c
 * @author Franz Luepke
 * @date   8 March 2021
 * @version 0.1
 * @brief  A Linux user space program that communicates with the avalon_enable.c LKM. It passes a
 * int to the LKM and the enable with this number. For this example to work the device
 * must be called /dev/avalon_enable.
*/
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>
// DEFINITIONS
#define DEVICES			1			// Number of devices of the same type
#define BUFFER_LENGTH	256			// The buffer length (crude but fine)
#define LOOP_DELAY		10000		// Delay in us
// Constants
const char *pathname = "/dev/avalon_enable";
const void* buffer;
// STATIC CHARS
static char receive[BUFFER_LENGTH];	// The receive buffer from the LKM
// MAIN FUNCTION
int main(int argc, char ** argv)
{
	int ret;
	int fd;

	int index;
	int value;
	// char buffer[BUFFER_LENGTH];
	// printf("Starting device test code example...\n");
	// Arguments
	if(argc != 3)
	{
		printf(" Please enter two arguments:  motor_select \t state\n");
		exit(EXIT_FAILURE);
	}
	index = atoi(argv[1]);
	value = atoi(argv[2]);
	
	// Open devices
	printf(" Open devices...\n");
	fd = open(pathname, O_RDWR); // Open the device with read/write access	
	if (fd < 0)
	{
		perror("  Failed to open the device.");
		return errno;
	}

	// // Writing operations
	// for (int i = 0; i < 2; ++i)
	// {
	// 	lseek(fd, i*4, SEEK_SET);
	// 	sprintf(buffer, "%d", 0);
	// 	printf("  Buffer: %s\n", buffer);
	// 	ret = write(fd, buffer, sizeof(buffer)); // Send the string to the LKM
	// 	if (ret < 0)
	// 	{
	// 		perror("   Failed to write the message to the device.");
	// 		return errno;
	// 	}
	// }
	
	// buffer = (const void*) index;
	// sprintf(buffer, "%d", 3);
	printf("  Index = %d\t Value: %d\n", index, value);
	// std::string s = std::to_string(msg->data);
	// int number = (int)strtol(s.c_str(), NULL, 10);
	// int number = (int) 10;
	// const void* buffer = (const void*) number;
	buffer = (const void*) value;
	lseek(fd, index*4, SEEK_SET);
	ret = write(fd, &buffer, sizeof(buffer)); // Send the string to the LKM
	if (ret < 0)
	{
		perror("   Failed to write the message to the device.");
		return errno;
	}
	// lseek(fd, 4, SEEK_SET);
	// // lseek(fd, 4, SEEK_CUR);
	// sprintf(buffer, "%d", state);
	// printf("  Buffer: %s\n", buffer);
	// ret = write(fd, buffer, sizeof(buffer)); // Send the string to the LKM
	// if (ret < 0)
	// {
	// 	perror("   Failed to write the message to the device.");
	// 	return errno;
	// }

	// Reading operations
	for (int i = 0; i < 3; ++i)
	{
		lseek(fd, i*4, SEEK_SET);
		ret = read(fd, receive, BUFFER_LENGTH);
		if(ret < 0)
		{
			perror("  Failed to read message from device");
		}
		printf("  Num: %d \t State value: [%d]\n", i*4, receive[0]);
	}


	// // Reading operations
	// for (int i = 0; i < 20; ++i)
	// {
	// 	lseek(fd, i, SEEK_SET);
	// 	ret = read(fd, receive, BUFFER_LENGTH);
	// 	if(ret < 0)
	// 	{
	// 		perror("  Failed to read message from device");
	// 	}
	// 	printf("  Num: %d \t State value: [%d]\n", i, receive[0]);
	// }




	// lseek(fd, 0, SEEK_SET);
	// ret = read(fd, receive, BUFFER_LENGTH);
	// if(ret < 0)
	// {
	// 	perror("  Failed to read message from device");
	// }
	// printf("  Motor value [%d]\n ", receive[0]);
	// lseek(fd, 4, SEEK_SET);
	// ret = read(fd, receive, BUFFER_LENGTH);
	// if(ret < 0)
	// {
	// 	perror("  Failed to read message from device");
	// }
	// printf("  State value [%d]\n", receive[0]);

	printf("End of the program\n");
	return 0;
}