/*
 * adxl345.h
 *
 *  Created on: 3 oct. 2014
 *      Author: ldo
 */

#ifndef ADXL345_H_
#define ADXL345_H_

#include <avr/io.h>

#include "twi.h"
#include "imu.h"

#define ADXL345_ADDRESS_LOW 0x53 // if SDO = 0 then ADXL345 7-bits address is 0x53
#define ADXL345_ADDRESS_HIGH 0x1D // if SDO = 1 then ADXL345 7-bits address is 0x1D

#define SENSITIVITY_2g 256.0 // for 2g, the sensitivity is 256 LSB/g
#define SENSITIVITY_4g 128.0 // for 16g, the sensitivity is 128 LSB/g
#define SENSITIVITY_8g 64.0 // for 16g, the sensitivity is 64 LSB/g
#define SENSITIVITY_16g 32.0 // for 16g, the sensitivity is 32 LSB/g

/* Register map */
#define DEVICE 0x00 // Device ID
#define OFSX 0x1E // X-axis offset
#define OFSY 0x1F // Y-axis offset
#define OFSZ 0x20 // Z-axis offset
#define POWER_CTL 0x2D // Power-saving features control
#define DATA_FORMAT 0x31 // Data format control
#define DATAX0 0x32 // X-axis Data 0 (LSB)
#define DATAX1 0x33 // X-axis Data 1 (MSB)
#define DATAY0 0x34 // Y-axis Data 0
#define DATAY1 0x35 // Y-axis Data 1
#define DATAZ0 0x36 // Z-axis Data 0
#define DATAZ1 0x37 // Z-axis Data 1

/* POWER_CTL register */
#define MEASURE 0x08

/* DATA FORMAT register */
#define G_RANGE_2g 0x00
#define G_RANGE_4g 0x01
#define G_RANGE_8g 0x02
#define G_RANGE_16g 0x03
#define FULL_RES 0x04
#define SELF_TEST 0x80

typedef enum
{
  MR_2_g = 0, MR_4_g = 1, MR_8_g = 2, MR_16_g = 3
} adxl345_measurement_range;

void
adxl345_setup (uint8_t sdo, adxl345_measurement_range mr);
void
adxl345_power_on (void);

xyz
adxl345_self_test (void);

uint8_t
adxl345_read_device_id (void);

void
adxl345_write_offset_xyz (xyz offset);
xyz
adxl345_read_offset_xyz (void);

void
adxl345_write_data_format (void);

xyz
adxl345_read_acc_xyz (void);

#endif /* ADXL345_H_ */
