#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <error.h>
#include <stdint.h>
#include <sys/mman.h>
// Header file
#include "../hps_0.h"
// The start address and length of the Lightweight bridge
#define HPS_TO_FPGA_LW_BASE 0xFF200000
#define HPS_TO_FPGA_LW_SPAN 0x0020000
int main(int argc, char ** argv)
{
	void * lw_bridge_map = 0;
	uint32_t * enable_map = 0;
	int value_0 = 0;
	int value_1 = 0;
	int value_2 = 0;
	int value_3 = 0;
	int value_4 = 0;
	int value_5 = 0;
	int value_6 = 0;
	int value_7 = 0;
	int value_8 = 0;
	int value_9 = 0;
	int devmem_fd = 0;
	int result = 0;
	int motor_select = 0;
	int state = 0;
	printf("Begin devmem_avalon_enable\n");

	// Check to make sure they entered a valid input value
	if(argc != 3)
	{
		printf(" Please enter only one argument that specifies the PWM value.\n");
		exit(EXIT_FAILURE);
	}
	// Get the PWM value from the passed in arguments
	motor_select	= atoi(argv[1]);
	state			= atoi(argv[2]);

	// Open up the /dev/mem device (aka, RAM)
	devmem_fd = open("/dev/mem", O_RDWR | O_SYNC);
	if(devmem_fd < 0)
	{
		perror(" devmem open");
		exit(EXIT_FAILURE);
	}
	// mmap() the entire address space of the Lightweight bridge so we can access our custom module 
	lw_bridge_map = (uint32_t*)mmap(NULL, HPS_TO_FPGA_LW_SPAN, PROT_READ|PROT_WRITE, MAP_SHARED, devmem_fd, HPS_TO_FPGA_LW_BASE); 
	if(lw_bridge_map == MAP_FAILED)
	{
		perror(" devmem mmap");
		close(devmem_fd);
		exit(EXIT_FAILURE);
	}
	printf(" --- HPS-2-FPGA LIGHTWEIGHT BRIDGE ADDRESSES ---\n");
	printf("  HPS_TO_FPGA_LW_BASE: [0x%08lx]\n", (long unsigned int) HPS_TO_FPGA_LW_BASE);
	printf("  lw_bridge_map: [0x%08lx]\n", (long unsigned int) lw_bridge_map);
	printf(" --- BASE ADDRESSES ---\n");
	printf("  AVALON_ENABLE_0_BASE: [0x%08lx]\n", (long unsigned int) AVALON_ENABLE_0_BASE);
	// Set the pwm_map to the correct offset within the RAM (AVALON_ENABLE_0_BASE is from "hps_0.h")
	enable_map = (uint32_t*)(lw_bridge_map + AVALON_ENABLE_0_BASE);
	printf(" --- MAPPED ADDRESSES ---\n");
	printf("  enable_map: [0x%08lx]\n", (long unsigned int) enable_map);
	// printf("  Motor: %d \t State: %d\n", motor_select, state);
	// // WRITING
	// printf("  Writing signals...\n");
	// printf("  motor_select value: %d motor_select state: %d\n", motor_select, state);
	// enable_map[0] = motor_select;
	// enable_map[1] = state;
	// READING
	printf("  Reading signals...\n");
	while(1)
	{
		value_0 = (int) enable_map[0];
		value_1 = (int) enable_map[1];
		value_2 = (int) enable_map[2];
		value_3 = (int) enable_map[3];
		value_4 = (int) enable_map[4];
		value_5 = (int) enable_map[5];
		value_6 = (int) enable_map[6];
		value_7 = (int) enable_map[7];
		value_8 = (int) enable_map[8];
		value_9 = (int) enable_map[9];
		printf("   Value read: [%d] [%d] [%d] [%d] [%d] [%d] [%d] [%d] [%d] [%d]\n", value_0, value_1, value_2, value_3, value_4, value_5, value_6, value_7, value_8, value_9);
		usleep(1000000);
	}
	printf(" Done!\n");
	// Unmap everything and close the /dev/mem file descriptor
	result = munmap(lw_bridge_map, HPS_TO_FPGA_LW_SPAN); 
	if(result < 0)
	{
		perror("   devmem munmap");
		close(devmem_fd);
		exit(EXIT_FAILURE);
	}
	close(devmem_fd);
	exit(EXIT_SUCCESS);
}