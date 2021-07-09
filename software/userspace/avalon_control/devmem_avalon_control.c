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
	uint32_t * control_0_map = 0;
	// uint32_t * control_1_map = 0;
	// uint32_t * control_2_map = 0;
	// uint32_t * control_3_map = 0;
	// uint32_t * control_4_map = 0;
	// uint32_t * control_5_map = 0;
	int dir_0_value = 0;
	int dir_1_value = 0;
	int dir_2_value = 0;
	int dir_3_value = 0;
	int dir_4_value = 0;
	int dir_5_value = 0;

	int devmem_fd = 0;
	int result = 0;
	int value = 0;
	// int KP_value = 0;
	// int KI_value = 0;
	// int KD_value = 0;

	// Check to make sure they entered a valid input value
	if(argc != 3)
	{
		printf("Please enter only one argument that specifies the PWM value.\n");
		exit(EXIT_FAILURE);
	}
	// Get the PWM value from the passed in arguments
	int index;
	index = atoi(argv[1]);
	value = atoi(argv[2]);

	// Open up the /dev/mem device (aka, RAM)
	devmem_fd = open("/dev/mem", O_RDWR | O_SYNC);
	if(devmem_fd < 0)
	{
		perror("devmem open");
		exit(EXIT_FAILURE);
	}
	// mmap() the entire address space of the Lightweight bridge so we can access our custom module 
	lw_bridge_map = (uint32_t*)mmap(NULL, HPS_TO_FPGA_LW_SPAN, PROT_READ|PROT_WRITE, MAP_SHARED, devmem_fd, HPS_TO_FPGA_LW_BASE); 
	if(lw_bridge_map == MAP_FAILED)
	{
		perror("devmem mmap");
		close(devmem_fd);
		exit(EXIT_FAILURE);
	}
	printf("--- HPS-2-FPGA LIGHTWEIGHT BRIDGE ADDRESSES ---\n");
	printf("  HPS_TO_FPGA_LW_BASE: [0x%08lx]\n", (long unsigned int) HPS_TO_FPGA_LW_BASE);
	printf("  lw_bridge_map: [0x%08lx]\n", (long unsigned int) lw_bridge_map);
	printf("--- BASE ADDRESSES ---\n");
	printf(" AVALON_CONTROL_0_BASE: [0x%08lx]\n", (long unsigned int) AVALON_CONTROL_0_BASE);
	// printf(" AVALON_CONTROL_1_BASE: [0x%08lx]\n", (long unsigned int) AVALON_CONTROL_1_BASE);
	// printf(" AVALON_CONTROL_2_BASE: [0x%08lx]\n", (long unsigned int) AVALON_CONTROL_2_BASE);
	// printf(" AVALON_CONTROL_3_BASE: [0x%08lx]\n", (long unsigned int) AVALON_CONTROL_3_BASE);
	// printf(" AVALON_CONTROL_4_BASE: [0x%08lx]\n", (long unsigned int) AVALON_CONTROL_4_BASE);
	// printf(" AVALON_CONTROL_5_BASE: [0x%08lx]\n", (long unsigned int) AVALON_CONTROL_5_BASE);
	// Set the pwm_map to the correct offset within the RAM (AVALON_PWM_0_BASE is from "hps_0.h")
	control_0_map = (uint32_t*)(lw_bridge_map + AVALON_CONTROL_0_BASE);
	// control_1_map = (uint32_t*)(lw_bridge_map + AVALON_CONTROL_1_BASE);
	// control_2_map = (uint32_t*)(lw_bridge_map + AVALON_CONTROL_2_BASE);
	// control_3_map = (uint32_t*)(lw_bridge_map + AVALON_CONTROL_3_BASE);
	// control_4_map = (uint32_t*)(lw_bridge_map + AVALON_CONTROL_4_BASE);
	// control_5_map = (uint32_t*)(lw_bridge_map + AVALON_CONTROL_5_BASE);
	printf("--- MAPPED ADDRESSES ---\n");
	printf(" control_0_map: [0x%08lx]\n", (long unsigned int) control_0_map);
	// printf(" control_1_map: [0x%08lx]\n", (long unsigned int) control_1_map);
	// printf(" control_2_map: [0x%08lx]\n", (long unsigned int) control_2_map);
	// printf(" control_3_map: [0x%08lx]\n", (long unsigned int) control_3_map);
	// printf(" control_4_map: [0x%08lx]\n", (long unsigned int) control_4_map);
	// printf(" control_5_map: [0x%08lx]\n", (long unsigned int) control_5_map);
	printf(" Changing all RPM outputs to: %d\n", value);
	control_0_map[index] = value;
	// control_1_map[index] = value;
	// control_2_map[index] = value;
	// control_3_map[index] = value;
	// control_4_map[index] = value;
	// control_5_map[index] = value;
	// int num = 0;
	while(1)
	{
		// dir_0_value = (int) control_0_map[num];
		// dir_1_value = (int) control_1_map[num];
		// dir_2_value = (int) control_2_map[num];
		// dir_3_value = (int) control_3_map[num];
		// dir_4_value = (int) control_4_map[num];
		// dir_5_value = (int) control_5_map[num];
		dir_0_value = (int) control_0_map[0];
		dir_1_value = (int) control_0_map[1];
		dir_2_value = (int) control_0_map[2];
		dir_3_value = (int) control_0_map[3];
		dir_4_value = (int) control_0_map[4];
		dir_5_value = (int) control_0_map[5];
		// printf(" Num: [%d] Value read: [%d] [%d] [%d] [%d] [%d] [%d]\n", num, dir_0_value, dir_1_value, dir_2_value, dir_3_value, dir_4_value, dir_5_value);
		printf(" Value read: [%d] [%d] [%d] [%d] [%d] [%d]\n", dir_0_value, dir_1_value, dir_2_value, dir_3_value, dir_4_value, dir_5_value);
		usleep(100000);
		// num++;
		// if(num==6)
		// {
		// 	num = 0;
		// 	printf("\n");
		// }
	}
	printf("Done!\n");
	// Unmap everything and close the /dev/mem file descriptor
	result = munmap(lw_bridge_map, HPS_TO_FPGA_LW_SPAN); 
	if(result < 0)
	{
		perror("devmem munmap");
		close(devmem_fd);
		exit(EXIT_FAILURE);
	}
	close(devmem_fd);
	exit(EXIT_SUCCESS);
}