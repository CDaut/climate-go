#ifndef NEW_CLIMTE_GO_SENSOR_DATA_T_H
#define NEW_CLIMTE_GO_SENSOR_DATA_T_H

#include <types.h>

/**
 * This struct represents data read from any sensor.
 * This implies, reading temperature from e.g. the CO2 Sensor
 * does not make any sense
 */
struct sensor_data_t {
    float temperature;
    float pressure;
};

#endif
