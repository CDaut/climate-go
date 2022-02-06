#include <Arduino.h>
#include <sensors/BmpSensor.h>

#define SLEEP_TIME 2
#define BAUD_RATE 112500

Sensor *bmpSensor;

void setup() {
    Serial.begin(BAUD_RATE);
    bmpSensor = new BmpSensor();
}

void loop() {
    sensor_data_t sample = ((BmpSensor *) bmpSensor)->sampleLowEnergy();
    Serial.printf("Temperature: %f | Pressure: %f \n",
                  sample.temperature,
                  sample.pressure
    );
    delay(SLEEP_TIME * 1000);
}