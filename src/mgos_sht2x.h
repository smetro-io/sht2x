/*
 * Copyright (c) 2017 Smetro Technologies
 * All rights reserved
 */

#ifndef CS_MOS_LIBS_SHT2x_SRC_MGOS_DHT_H_
#define CS_MOS_LIBS_SHT2x_SRC_MGOS_DHT_H_

#include "mgos_i2c.h"

#ifdef __cplusplus
extern "C" {
#endif


/* Returns temperature in DegC or 'NAN' if operation failed. */
float mgos_sht2x_get_temp(struct mgos_i2c *i2c);
/* Returns humidity in % or 'NAN' if operation failed. */
float mgos_sht2x_get_humidity(struct mgos_i2c *i2c);

bool mgos_sht2x_init(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* CS_MOS_LIBS_SHT2x_SRC_MGOS_DHT_H_ */
