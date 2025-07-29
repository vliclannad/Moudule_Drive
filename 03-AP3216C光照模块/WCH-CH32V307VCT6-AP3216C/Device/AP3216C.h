#ifndef __AP3216C_H
#define __AP3216C_H

#include "debug.h"

#define AP3216C_ADDR    0X1E	//AP3216CÆ÷¼þIICµØÖ·

/*
0x00 System Configuration Control of basic functions
0x01 Interrupt Status ALS and PS interrupt status output
0x02 INT Clear Manner Auto/semi clear INT pin selector
0x0A IR Data Low Lower byte for IR ADC channel output
0x0B IR Data High Higher byte for IR ADC channel output
0x0C ALS Data Low Lower byte for ALS ADC channel output
0x0D ALS Data High Higher byte for ALS ADC channel output
0x0E PS Data Low Lower byte for PS ADC channel output
0x0F PS Data High Higher byte for PS ADC channel output
 */



void AP3216C_Init(void);
void AP3216C_ReadData(uint16_t* alsData);
#endif
