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
	uint32_t * rpm_0_map = 0;
	// uint32_t * rpm_1_map = 0;
	// uint32_t * rpm_2_map = 0;
	// uint32_t * rpm_3_map = 0;
	// uint32_t * rpm_4_map = 0;
	// uint32_t * rpm_5_map = 0;
	int devmem_fd = 0;
	int result = 0;
	int rpm_0_value = 0;
	// int rpm_1_value = 0;
	// int rpm_2_value = 0;
	// int rpm_3_value = 0;
	// int rpm_4_value = 0;
	// int rpm_5_value = 0;
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
	// Set the rpm_map to the correct offset within the RAM (AVALON_RPM_X_BASE is from "hps_0.h")
	rpm_0_map = (uint32_t*)(lw_bridge_map + AVALON_RPM_0_BASE);
	// rpm_1_map = (uint32_t*)(lw_bridge_map + AVALON_RPM_1_BASE);
	// rpm_2_map = (uint32_t*)(lw_bridge_map + AVALON_RPM_2_BASE);
	// rpm_3_map = (uint32_t*)(lw_bridge_map + AVALON_RPM_3_BASE);
	// rpm_4_map = (uint32_t*)(lw_bridge_map + AVALON_RPM_4_BASE);
	// rpm_5_map = (uint32_t*)(lw_bridge_map + AVALON_RPM_5_BASE);
	printf(" Reading all RPM outputs...\n");
	while(1)
	{
		printf(" Value read:");
		for (int i = 0; i < 40; ++i)
		{
			rpm_0_value = (int) rpm_0_map[i];
			printf(" [%d]", rpm_0_value);
		}
		printf("\n");
		// rpm_0_value = (int) rpm_0_map[0];
		// rpm_1_value = (int) rpm_0_map[4];
		// rpm_2_value = (int) rpm_0_map[8];
		// rpm_3_value = (int) rpm_0_map[12];
		// rpm_5_value = (int) rpm_0_map[16];

		// rpm_1_value = (int) rpm_1_map[0];
		// rpm_2_value = (int) rpm_2_map[0];
		// rpm_3_value = (int) rpm_3_map[0];
		// rpm_4_value = (int) rpm_4_map[0];
		// rpm_5_value = (int) rpm_5_map[0];
		// printf("  Value read: [%x] [%x] [%x] [%x] [%x] [%x]\n", rpm_0_value, rpm_1_value, rpm_2_value, rpm_3_value, rpm_4_value, rpm_5_value);
		// printf("\r \33[2K Value read: [%x] [%x] [%x] [%x] [%x] [%x]", rpm_0_value, rpm_1_value, rpm_2_value, rpm_3_value, rpm_4_value, rpm_5_value);
		// fflush(stdout);
		// Wait half a second
		usleep(1000000);
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