// Mongoose OS SHT2x library API. Source C API is defined at:
// [mgos_sht2x.h](https://github.com/smetro-io/sht2x/tree/master/src/mgos_sht2x.h)

let SHT2x = {
  _gt: ffi('float mgos_sht2x_get_temp(void *)'),
  _gh: ffi('float mgos_sht2x_get_humidity(void *)'),

  // **`SHT2x.getTemp(i2c)`**
  // Returns temperature in DegC
  // or 'NaN' if operation failed
  getTemp: function(i2c) {
    return SHT2x._gt(i2c);
  },

  // **`SHT2x.getHumidity(i2c)`**
  // Returns temperature in RH%
  // or 'NaN' if operation failed.
  getHumidity: function(i2c) {
    return SHT2x._gh(i2c);
  },
};
