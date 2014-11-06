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

static uint16_t sensitivity = SENSITIVITY_16g;
static uint8_t slave_address = ADXL345_ADDRESS_LOW;

static xyz low_pass;

/*
 * \fn void adxl345_setup (void)
 * \brief adxl345 setup function
 * \param sdo select slave address
 * \param mr full-scale range (2, 4, 8 or 16 g)
 * */
void
adxl345_setup (uint8_t sdo, adxl345_measurement_range mr)
{
  volatile uint8_t buffer[2];

  twi_master_setup (/*SCL_FREQ_100*/);

  if (sdo == 0)
    {
      slave_address = ADXL345_ADDRESS_LOW;
    }
  else
    {
      slave_address = ADXL345_ADDRESS_HIGH;
    }

  /* set the g range (2, 4, 8 or 16 g) */
  switch (mr)
    {
    case MR_2_g:
      buffer[0] = DATA_FORMAT;
      buffer[1] = G_RANGE_2g;
      twi_write_bytes (slave_address, 2, buffer);
      sensitivity = SENSITIVITY_2g;
      break;
    case MR_4_g:
      buffer[0] = DATA_FORMAT;
      buffer[1] = G_RANGE_4g;
      twi_write_bytes (slave_address, 2, buffer);
      sensitivity = SENSITIVITY_4g;
      break;
    case MR_8_g:
      buffer[0] = DATA_FORMAT;
      buffer[1] = G_RANGE_8g;
      twi_write_bytes (slave_address, 2, buffer);
      sensitivity = SENSITIVITY_8g;
      break;
    case MR_16_g:
    default:
      buffer[0] = DATA_FORMAT;
      buffer[1] = G_RANGE_16g;
      twi_write_bytes (slave_address, 2, buffer);
      sensitivity = SENSITIVITY_16g;
      break;
    }

  low_pass.x = 0.0;
  low_pass.y = 0.0;
  low_pass.z = 0.0;
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
  twi_write_bytes (slave_address, 2, buffer);
}

/*
 * \fn uint8_t adxl345_read_device_id (void)
 * \brief read device id
 * \return a fixed device id code of 0xE5
 * */
uint8_t
adxl345_read_device_id (void)
{
  volatile uint8_t reg = DEVICE;
  volatile uint8_t device_id;

  twi_read_bytes (slave_address, &reg, 1, &device_id);

  return device_id;
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
  twi_write_bytes (slave_address, 2, buffer);

  /* read x, y and y value */
  acc = adxl345_read_acc_xyz ();

  /* self-test function is disable (SELF_TEST bit in DATA_FORMAT is reset) */
  buffer[0] = DATA_FORMAT;
  buffer[1] = G_RANGE_16g;
  twi_write_bytes (slave_address, 2, buffer);

  return acc;
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

  twi_read_bytes (slave_address, &reg, 3, buffer);

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

  twi_write_bytes (slave_address, 4, buffer);
}

/**
 * \brief low pass filter y(t) = a.xm(t) – (1-a).y(t-1)
 * */
void
low_pass_filter (xyz *measure)
{
  double alpha = 0.2;

  low_pass.x = measure->x * alpha + (low_pass.x * (1.0 - alpha));
  low_pass.y = measure->y * alpha + (low_pass.y * (1.0 - alpha));
  low_pass.z = measure->z * alpha + (low_pass.z * (1.0 - alpha));

  *measure = low_pass;
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
  xyz acc;

  twi_read_bytes (slave_address, &reg, 6, buffer);

  acc.x = (double) (buffer[0] + (buffer[1] << 8)) / sensitivity;
  acc.y = (double) (buffer[2] + (buffer[3] << 8)) / sensitivity;
  acc.z = (double) (buffer[4] + (buffer[5] << 8)) / sensitivity;

  low_pass_filter (&acc);

  return acc;
}

