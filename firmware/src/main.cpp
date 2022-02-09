#include <Arduino.h>
#include <sensors/BmpSensor.h>
#include <ble/BluetoothServer.h>

#define SLEEP_TIME 1
#define BAUD_RATE 112500
#define INTERNAL_LED_PIN 2
#define INTERRUPT_MAKE_DISCOVERABLE_PIN 15


Sensor *bmpSensor;
BluetoothServer *server;
bool makeDiscoverable = false;


void IRAM_ATTR startBleServerAdvertising() {
    makeDiscoverable = true;
}

void setup() {
    Serial.begin(BAUD_RATE);
    pinMode(INTERNAL_LED_PIN, OUTPUT);

    bmpSensor = new BmpSensor();

    server = new BluetoothServer();
    server->startServer();

    //set up Interrupt to enable make discoverable button
    pinMode(INTERRUPT_MAKE_DISCOVERABLE_PIN, INPUT_PULLDOWN);
    attachInterrupt(digitalPinToInterrupt(INTERRUPT_MAKE_DISCOVERABLE_PIN),
                    startBleServerAdvertising,
                    RISING);
}

void loop() {
    if (makeDiscoverable) {
        server->startAdvertising();
        makeDiscoverable = false;
    }

    sensor_data_t sample = ((BmpSensor *) bmpSensor)->sampleLowEnergy();
    server->setPressure(sample.pressure);
    server->setTemperature(sample.temperature);
    delay(SLEEP_TIME * 1000);
}