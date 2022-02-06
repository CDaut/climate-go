#include <Arduino.h>
#include <sensors/BmpSensor.h>

#define SLEEP_TIME 2

Sensor *bmpSensor;

void setup() {
    bmpSensor = new BmpSensor();
}

void loop() {
    bmpSensor->sample();
    bmpSensor->enableStandbyMode();
    delay(SLEEP_TIME * 1000);
}