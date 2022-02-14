#ifndef FIRMWARE_SENSOR_DATA_T_H
#define FIRMWARE_SENSOR_DATA_T_H

/**
 * This struct represents data read from any sensor.
 * This implies, reading temperature from e.g. the CO2 Sensor
 * does not make any sense
 */
struct sensor_data_t {
    float temperature;
    float pressure;
    float pm10;
    float pm25;
};

#endif
