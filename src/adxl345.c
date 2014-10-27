/*
 * adxl345.c
 *
 *  Created on: 3 oct. 2014
 *      Author: ldo
 *
 *  ADXL345 is a 3-axis Accelerometer with high resolution (13 bits) measurement at up +/- 16g.
 *  Digital output are accessible through either a SPI or I2C digital interface.
 */

#include "adxl345.h"

uint16_t sensitivity;

/*
 * \fn void adxl345_setup (void)
 * \brief adxl345 setup function
 * */
void
adxl345_setup (void)
{
  twi_master_setup (/*SCL_FREQ_100*/);
}

/*
 * \fn void adxl345_power_on (void)
 * \brief setup measurement mode and sensitivity
 * */
void
adxl345_power_on (void)
{
  volatile uint8_t buffer[2];

  /* setting of 1 in the measure bit places the part into measurement mode */
  buffer[0] = POWER_CTL;
  buffer[1] = MEASURE;
  twi_write_bytes (ADXL345_ADDRESS, 2, buffer);

  /* these bits set the g range (2, 4, 8 or 16 g) */
  buffer[0] = DATA_FORMAT;
  buffer[1] = G_RANGE_16g;
  twi_write_bytes (ADXL345_ADDRESS, 2, buffer);

  sensitivity = SENSITIVITY_16g; /* sensitivity for 16g full_scale range */
}

/*
 * \fn xyz adxl345_self_test (void)
 * \brief self test (g range +/- 16g, result right justified, 10-bits resolution)
 * \return x, y, z test values [g] */
xyz
adxl345_self_test (void)
{
  volatile uint8_t buffer[2];
  volatile xyz acc;

  /* self-test function is enable (SELF_TEST bit in DATA_FORMAT is set) */
  buffer[0] = DATA_FORMAT;
  buffer[1] = SELF_TEST + G_RANGE_16g;
  twi_write_bytes (ADXL345_ADDRESS, 2, buffer);

  /* read x, y and y value */
  acc = adxl345_read_acc_xyz();

  /* self-test function is disable (SELF_TEST bit in DATA_FORMAT is reset) */
  buffer[0] = DATA_FORMAT;
  buffer[1] = G_RANGE_16g;
  twi_write_bytes (ADXL345_ADDRESS, 2, buffer);

  return acc;
}

/*
 * \fn uint8_t adxl345_read_device_id (void)
 * \brief read device id
 * \return device id
 * */
uint8_t
adxl345_read_device_id (void)
{
  volatile uint8_t reg = DEVICE;
  volatile uint8_t device_id;

  twi_read_bytes (ADXL345_ADDRESS, &reg, 1, &device_id);

  return device_id;
}

/*
 * \fn xyz adxl345_read_offset_xyz (void)
 * \brief read x, y and z offset value
 * \return x, y and z offset value
 * */
xyz
adxl345_read_offset_xyz (void)
{
  volatile uint8_t reg = OFSX;
  volatile uint8_t buffer[3];
  volatile xyz offset;

  twi_read_bytes (ADXL345_ADDRESS, &reg, 3, buffer);

  offset.x = buffer[0];
  offset.y = buffer[1];
  offset.z = buffer[2];

  return offset;
}

/*
 * \fn void adxl345_write_offset_xyz (xyz offset)
 * \brief write x, y and z offset value
 * \param x, y and z offset value
 * */
void
adxl345_write_offset_xyz (xyz offset)
{
  volatile uint8_t buffer[4];

  buffer[0] = OFSX;
  buffer[1] = offset.x;
  buffer[2] = offset.y;
  buffer[3] = offset.z;

  twi_write_bytes (ADXL345_ADDRESS, 4, buffer);
}

/*
 * \fn xyz adxl345_read_acc_xyz (void)
 * \brief read x,y and z acceleration value
 * \return x, y and z acceleration [g]
 * */
xyz
adxl345_read_acc_xyz (void)
{
  volatile uint8_t reg = DATAX0;
  volatile uint8_t buffer[6];
  volatile xyz acc;

  twi_read_bytes (ADXL345_ADDRESS, &reg, 6, buffer);

  acc.x = (double) (buffer[0] + (buffer[1] << 8)) / sensitivity;
  acc.y = (double) (buffer[2] + (buffer[3] << 8)) / sensitivity;
  acc.z = (double) (buffer[4] + (buffer[5] << 8)) / sensitivity;

  return acc;
}

