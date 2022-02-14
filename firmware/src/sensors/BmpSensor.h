#ifndef NEW_CLIMTE_GO_BMPSENSOR_H
#define NEW_CLIMTE_GO_BMPSENSOR_H

#include "Sensor.h"
#include "Adafruit_BMP280.h"

class BmpSensor : public Sensor {
private:
    Adafruit_BMP280 bmp; // use I2C interface
    Adafruit_Sensor *bmp_temp = bmp.getTemperatureSensor();
    Adafruit_Sensor *bmp_pressure = bmp.getPressureSensor();

public:
    BmpSensor();

    sensor_data_t sample() override;

    sensor_data_t sampleLowEnergy();

    void enableStandbyMode() override;

    void wakeUp() override;

protected:
    void sensor_setup() override;

};

#endif //NEW_CLIMTE_GO_BMPSENSOR_H

