/*
 * Copyright (c) 2017 Smetro Technologies
 * All rights reserved
 */

#include <stdint.h>
#include <math.h>

#include "fw/src/mgos_hal.h"
#include "mgos_sht2x.h"

/* I2C address */
#define SHT2x_I2C_ADDRESS 0x40

/* I2C command bytes */
#define SHT2x_TRIG_T_MEASUREMENT_HM  0xe3
#define SHT2x_TRIG_RH_MEASUREMENT_HM 0xe5
#define SHT2x_TRIG_T_MEASUREMENT_NHM  0xf3
#define SHT2x_TRIG_RH_MEASUREMENT_NHM 0xf5

/* Status bits included in the measurement need to be masked in calculation */
#define STATUS_BITS_MASK 0x0003

/* An 8-bit CRC is used to validate the readings */
#define CRC8_POLYNOMIAL 0x131
#define CRC8_INITIAL_CRC 0x00

/* Return an updated checksum for the given crc and byte */
static uint8_t crc8(uint8_t crc, uint8_t byte) {
  uint8_t i;

  crc ^= byte;

  for (i = 0; i < 8; i++) {
    if ((crc & 0x80) == 0x80) {
      crc = (crc << 1) ^ CRC8_POLYNOMIAL;
    } else {
      crc <<= 1;
    }
  }

  return crc;
}

/*
 * Compute the CRC of an array of bytes and compare it to expected_crc.
 * If the computed CRC matches expected_crc, then return true, otherwise false.
 */
static bool checksum(uint8_t *bytes, int nbytes, uint8_t expected_crc) {
  int i;
  uint8_t crc;

  crc = CRC8_INITIAL_CRC;

  for (i = 0; i < nbytes; i++) {
    crc = crc8(crc, bytes[i]);
  }

  if (crc != expected_crc) {
    LOG(LL_WARN, ("Bad SHT2x CRC -- Computed CRC: 0x%x | Expected CRC: 0x%x\n",
      crc, expected_crc));
    return false;
  }
  return true;
}

static bool sht2x_read(struct mgos_i2c *i2c, uint8_t cmd, uint16_t *val) {
  uint8_t data[3] = { 0 };
  if (!mgos_i2c_write(i2c, SHT2x_I2C_ADDRESS, &cmd, 1, true /* stop */)) {
    return false;
  }
  mgos_msleep(100);
  if (!mgos_i2c_read(i2c, SHT2x_I2C_ADDRESS, data, 3, true /* stop */)) {
    return false;
  }
  mgos_msleep(100);
  if (!checksum(data, 2, data[2])) {
    return false;
  }
  *val = ((data[0] << 8) + data[1]) & ~STATUS_BITS_MASK;
  return true;
}

float mgos_sht2x_get_temp(struct mgos_i2c *i2c) {
  uint16_t val;
  if (!sht2x_read(i2c, SHT2x_TRIG_T_MEASUREMENT_NHM, &val)) {
    return NAN;
  }

  /* Formula T = -46.85 + 175.72 * ST / 2^16 from data sheet 6.2 */
  return (float)(-46.85 + ((175.72 / 65536) * ((float) val)));
}

float mgos_sht2x_get_humidity(struct mgos_i2c *i2c) {
  uint16_t val;
  if (!sht2x_read(i2c, SHT2x_TRIG_RH_MEASUREMENT_NHM, &val)) {
    return NAN;
  }
  /* Formula RH = -6 + 125 * SRH / 2^16 from data sheet 6.1 */
  return (float) (-6.0 + ((125.0 / 65536) * ((float) val)));
}

bool mgos_sht2x_init(void) {
  return true;
}
