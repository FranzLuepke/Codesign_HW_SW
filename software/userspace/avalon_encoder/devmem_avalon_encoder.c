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
	uint32_t * encoder_0_map = 0;
	uint32_t * encoder_1_map = 0;
	uint32_t * encoder_2_map = 0;
	uint32_t * encoder_3_map = 0;
	uint32_t * encoder_4_map = 0;
	uint32_t * encoder_5_map = 0;
	int devmem_fd = 0;
	int result = 0;
	int encoder_0_value = 0;
	int encoder_1_value = 0;
	int encoder_2_value = 0;
	int encoder_3_value = 0;
	int encoder_4_value = 0;
	int encoder_5_value = 0;
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
	// Set the encoder_x_map to the correct offset within the RAM (AVALON_PWM_X_BASE is from "hps_0.h")
	encoder_0_map = (uint32_t*)(lw_bridge_map + AVALON_ENCODER_0_BASE);
	encoder_1_map = (uint32_t*)(lw_bridge_map + AVALON_ENCODER_1_BASE);
	encoder_2_map = (uint32_t*)(lw_bridge_map + AVALON_ENCODER_2_BASE);
	encoder_3_map = (uint32_t*)(lw_bridge_map + AVALON_ENCODER_3_BASE);
	encoder_4_map = (uint32_t*)(lw_bridge_map + AVALON_ENCODER_4_BASE);
	encoder_5_map = (uint32_t*)(lw_bridge_map + AVALON_ENCODER_5_BASE);
	while(1)
	{
		// Read encoders
		encoder_0_value = (int) encoder_0_map[0];
		encoder_1_value = (int) encoder_1_map[1];
		encoder_2_value = (int) encoder_2_map[0];
		encoder_3_value = (int) encoder_3_map[0];
		encoder_4_value = (int) encoder_4_map[0];
		encoder_5_value = (int) encoder_5_map[0];
        printf("\r \33[2K Value read: [%x] [%x] [%x] [%x] [%x] [%x]", encoder_0_value, encoder_1_value, encoder_2_value, encoder_3_value, encoder_4_value, encoder_5_value);
        fflush(stdout);
		// Wait half a second
		usleep(1000);
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