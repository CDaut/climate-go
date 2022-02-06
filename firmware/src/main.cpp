#include <Arduino.h>
#include <sensors/BmpSensor.h>
#include <ble/BluetoothServer.h>

#define SLEEP_TIME 2
#define BAUD_RATE 112500
#define INTERNAL_LED_PIN 2


Sensor *bmpSensor;
BluetoothServer *server;

void setup() {
    Serial.begin(BAUD_RATE);
    pinMode(INTERNAL_LED_PIN, OUTPUT);

    bmpSensor = new BmpSensor();

    server = new BluetoothServer();
    server->startServer();
}

void loop() {
    sensor_data_t sample = ((BmpSensor *) bmpSensor)->sampleLowEnergy();
    server->setPressure(sample.pressure);
    server->setTemperature(sample.temperature);
    delay(SLEEP_TIME * 1000);
}