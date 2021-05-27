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
	uint32_t * control_1_map = 0;
	uint32_t * control_2_map = 0;
	uint32_t * control_3_map = 0;
	uint32_t * control_4_map = 0;
	uint32_t * control_5_map = 0;
	int devmem_fd = 0;
	int result = 0;
	int RPM_value = 0;
	// int KP_value = 0;
	// int KI_value = 0;
	// int KD_value = 0;
	// Check to make sure they entered a valid input value
	if(argc != 2)
	{
		printf("Please enter only one argument that specifies the PWM value.\n");
		exit(EXIT_FAILURE);
	}
	// Get the PWM value from the passed in arguments
	RPM_value = atoi(argv[1]);
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
	// Set the pwm_map to the correct offset within the RAM (AVALON_PWM_0_BASE is from "hps_0.h")
	control_0_map = (uint32_t*)(lw_bridge_map + AVALON_CONTROL_0_BASE);
	control_1_map = (uint32_t*)(lw_bridge_map + AVALON_CONTROL_1_BASE);
	control_2_map = (uint32_t*)(lw_bridge_map + AVALON_CONTROL_2_BASE);
	control_3_map = (uint32_t*)(lw_bridge_map + AVALON_CONTROL_3_BASE);
	control_4_map = (uint32_t*)(lw_bridge_map + AVALON_CONTROL_4_BASE);
	control_5_map = (uint32_t*)(lw_bridge_map + AVALON_CONTROL_5_BASE);
	printf(" Changing all PWM outputs to: %d\n", RPM_value);
	*control_0_map = RPM_value;
	*control_1_map = RPM_value;
	*control_2_map = RPM_value;
	*control_3_map = RPM_value;
	*control_4_map = RPM_value;
	*control_5_map = RPM_value;
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