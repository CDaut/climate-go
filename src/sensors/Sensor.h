#ifndef NEW_CLIMTE_GO_SENSOR_H
#define NEW_CLIMTE_GO_SENSOR_H

#include <sensors/sensor_data_t.h>

enum class SensorState : unsigned short {
    AWAKE, ASLEEP
};

class Sensor {
public:
    virtual sensor_data_t sample() = 0;

    virtual void wakeUp() = 0;

    virtual void enableStandbyMode() = 0;

protected:
    SensorState state = SensorState::ASLEEP;

    virtual void sensor_setup() = 0;

};

#endif //NEW_CLIMTE_GO_SENSOR_H
