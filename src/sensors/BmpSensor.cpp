#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_BMP280.h>
#include "BmpSensor.h"

/**
 * Creates a new BMP Sensor and initializes it
 */
BmpSensor::BmpSensor() {
    this->BmpSensor::sensor_setup();
    this->state = SensorState::ASLEEP;
}

/**
 * Function responsible for setting up the sensor and the I2C connection
 */
void BmpSensor::sensor_setup() {
    while (!Serial) delay(100);   // wait for native usb

    unsigned status = this->bmp.begin(0x76); //set the correct I2C port

    //query status and reboot the board if no sensor is detected
    if (!status)
        ESP.restart();

    //put sensor to standby mode
    this->enableStandbyMode();
}

/**
 * Function to wake sensor up from standby mode
 */
void BmpSensor::wakeUp() {
    this->bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                          Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                          Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                          Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                          Adafruit_BMP280::STANDBY_MS_500 /* Standby time. */
    );
}

/**
 * Function to put sensor to standby mode
 */
void BmpSensor::enableStandbyMode() {
    this->bmp.setSampling(Adafruit_BMP280::MODE_SLEEP,     /* Operating Mode. */
                          Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                          Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                          Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                          Adafruit_BMP280::STANDBY_MS_500 /* Standby time. */
    );
}

/**
 * read a sample from the sensor
 * @return a struct containing all necessary sensor data
 */
sensor_data_t BmpSensor::sample() {
    //wake sensor up if it is in standby mode
    if (this->state == SensorState::ASLEEP)
        this->wakeUp();

    //sample pressure and temperature
    sensors_event_t temp_event, pressure_event;
    this->bmp_temp->getEvent(&temp_event);
    this->bmp_pressure->getEvent(&pressure_event);


    return sensor_data_t
            {
                    temp_event.temperature,
                    pressure_event.pressure
            };
}

/**
 * Read a sample and put sensor to standby mode
 * @return the sample read
 */
sensor_data_t BmpSensor::sampleLowEnergy() {
    //read sample
    sensor_data_t sampledData = this->sample();
    //put sensor to standby
    this->enableStandbyMode();
    return sampledData;
}


