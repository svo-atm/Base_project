#include "RTE_Components.h"
#include CMSIS_device_header
#include <stdio.h>

#include "device_cfg.h"
#include "device_definition.h"
#include "arm_mps3_io_drv.h"

#include "arm_mps3_io_reg_map.h"

extern int stdout_init();



int main() {
    __enable_irq();
    stdout_init();
    printf("Bare metal hello!!!!\r\n");

    struct arm_mps3_io_reg_map_t* p_mps3_io_port =
                                (struct arm_mps3_io_reg_map_t*)MPS3_IO_DEV.cfg->base; // MPS3 IO base address

    uint8_t sw_mask = p_mps3_io_port->fpgaio_switches; 

    for (;;) {
        if(sw_mask != (uint8_t)p_mps3_io_port->fpgaio_switches)
        {
            printf("Switches changed: %02X\r\n", (uint8_t)p_mps3_io_port->fpgaio_switches);
            sw_mask = (uint8_t)p_mps3_io_port->fpgaio_switches;
            p_mps3_io_port->fpgaio_leds = sw_mask;
        }
    }
}
