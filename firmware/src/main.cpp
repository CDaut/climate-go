#include <Arduino.h>
#include <sensors/BmpSensor.h>
#include <ble/BluetoothServer.h>
#include <sensors/PmSensor.h>

#define SLEEP_TIME 1
#define BAUD_RATE 112500
#define INTERNAL_LED_PIN 2
#define INTERRUPT_MAKE_DISCOVERABLE_PIN 15


Sensor *bmpSensor;
Sensor *pmSensor;
BluetoothServer *server;
bool makeDiscoverable = false;


void IRAM_ATTR startBleServerAdvertising() {
    makeDiscoverable = true;
}

void setup() {
    //initialize a Serial Connection
    Serial.begin(BAUD_RATE);
    //setup the internal LED
    pinMode(INTERNAL_LED_PIN, OUTPUT);

    //create sensor objects
    bmpSensor = new BmpSensor();
    pmSensor = new PmSensor();

    //setup & start Bluetooth server
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

    //setup PM sensor measurement TODO: This blocks for 120 seconds and needs to be executed asynchronously
    //((PmSensor *) pmSensor)->startAsyncSampling();

    //obtain measurement from BMP280 Sensor
    sensor_data_t sample = ((BmpSensor *) bmpSensor)->sampleLowEnergy();
    server->setPressure(sample.pressure);
    server->setTemperature(sample.temperature);
    //Obtain measurement from PM Sensor
    sensor_data_t pmSample = pmSensor->sample();

    //Print measured data
    Serial.printf("Pressure: %f hPa | Temperature: %f °C | PM10: %f ppm| PM2.5: %f ppm\n",
                  sample.pressure,
                  sample.temperature,
                  pmSample.pm10,
                  pmSample.pm25);
    //wait for one measurement cycle
    delay(SLEEP_TIME * 1000);
}