#include "PmSensor.h"
#include <SoftwareSerial.h>
#include <Sds011.h>

#define SDS_PIN_RX 18
#define SDS_PIN_TX 19
#define SENSOR_BAUD_RATE 9600
#define SLEEP_TIME 90
#define MEASURE_TIME 30

#ifdef ESP32
HardwareSerial &serialSDS(Serial2);
Sds011Async<HardwareSerial> sds011(serialSDS);
#else
SoftwareSerial serialSDS;
Sds011Async< SoftwareSerial > sds011(serialSDS);
#endif


PmSensor::PmSensor() {
    this->PmSensor::sensor_setup();
}

void PmSensor::startAsyncSampling() {

    this->enableStandbyMode();
    PmSensor::sensorWait(SLEEP_TIME);

    this->wakeUp();
    sds011.perform_work();

    if (!sds011.query_data_auto_async(pm_tablesize, pm25_table, pm10_table)) {
        Serial.println("measurement capture start failed");
    }

    PmSensor::sensorWait(MEASURE_TIME);
}

/**
 * Put the sensor to standby mode
 */
void PmSensor::enableStandbyMode() {
    if (sds011.set_sleep(true)) { this->state = SensorState::ASLEEP; }
}

/**
 * Activate the sensor
 */
void PmSensor::wakeUp() {
    if (sds011.set_sleep(false)) { this->state = SensorState::AWAKE; }
}

/**
 * Method to set up and initialize the sensor
 */
void PmSensor::sensor_setup() {
#ifdef ESP32
    serialSDS.begin(SENSOR_BAUD_RATE, SERIAL_8N1, SDS_PIN_RX, SDS_PIN_TX);
    delay(100);
#else
    serialSDS.begin(SENSOR_BAUD_RATE, SWSERIAL_8N1, SDS_PIN_RX, SDS_PIN_TX, false, 192);
#endif

    //put the sensor into standby mode initially
    this->PmSensor::enableStandbyMode();
    Serial.println("SDS011 sensor initialized to standby mode.");

    //attach a callback on what to do when data sampling is complete
    sds011.on_query_data_auto_completed([this](int n) {
        int pm25;
        int pm10;
        if (sds011.filter_data(n, pm25_table, pm10_table, pm25, pm10) &&
            !isnan(pm10) && !isnan(pm25)) {
            //set the right values
            this->latestPM10 = float(pm10) / 10;
            this->latestPM25 = float(pm25) / 10;
        }
    });
}

/**
 * Method to wait for some time and perform the appropriate task
 * @param time the time to wait for in seconds
 */
void PmSensor::sensorWait(int time) {
    uint32_t deadline = millis() + time * 1000;
    while (static_cast<int32_t>(deadline - millis()) > 0) {
        delay(1000);
        sds011.perform_work();
    }
}

sensor_data_t PmSensor::sample() {
    return {0.0, 0.0, this->latestPM10, this->latestPM25};
}
