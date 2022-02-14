#ifndef FIRMWARE_PMSENSOR_H
#define FIRMWARE_PMSENSOR_H

#include "Sensor.h"
#include "sensor_data_t.h"

class PmSensor : public Sensor {
public:
    static const int pm_tablesize = 20;
    int pm25_table[pm_tablesize];
    int pm10_table[pm_tablesize];

    float latestPM10;
    float latestPM25;

    PmSensor();

    sensor_data_t sample() override;

    void startAsyncSampling();


    void enableStandbyMode() override;

    void wakeUp() override;

protected:
    void sensor_setup() override;

private:
    static void sensorWait(int time);
};


#endif //FIRMWARE_PMSENSOR_H
