#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_BMP280.h>
#include "BmpSensor.h"

BmpSensor::BmpSensor() {
    this->BmpSensor::sensor_setup();
    this->state = SensorState::ASLEEP;
}

void BmpSensor::sensor_setup() {
    Serial.begin(112500);
    while (!Serial) delay(100);   // wait for native usb

    unsigned status;
    status = this->bmp.begin(0x76); //set the correct I2C port

    //query status and reboot the board if no sensor is detected
    if (!status) {
        ESP.restart();

    }

/* Default settings from datasheet. */
    this->enableStandbyMode();
}

void BmpSensor::wakeUp() {
    this->bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                          Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                          Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                          Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                          Adafruit_BMP280::STANDBY_MS_500 /* Standby time. */
    );
}

void BmpSensor::enableStandbyMode() {
    this->bmp.setSampling(Adafruit_BMP280::MODE_SLEEP,     /* Operating Mode. */
                          Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                          Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                          Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                          Adafruit_BMP280::STANDBY_MS_500 /* Standby time. */
    );
}

void BmpSensor::sample() {
    //wake sensor up if it is in standby mode
    if (this->state == SensorState::ASLEEP)
        this->wakeUp();

    sensors_event_t temp_event, pressure_event;
    this->bmp_temp->getEvent(&temp_event);
    bmp_pressure->getEvent(&pressure_event);

    Serial.print(F("Temperature = "));
    Serial.print(temp_event.temperature);
    Serial.print(" °C | ");

    Serial.print(F("Pressure = "));
    Serial.print(pressure_event.pressure);
    Serial.println(" hPa");
}


