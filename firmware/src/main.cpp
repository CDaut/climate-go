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

//interrupt handler for sensor being pressed
void IRAM_ATTR startBleServerAdvertising() {
    makeDiscoverable = true;
}

//helper function to create task for PM measurement
void startPmAsyncSampling(void * param){
    ((PmSensor *) pmSensor)->startAsyncSampling();
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

    //start an async task for measuring PM so the sensor won't block the whole system
    xTaskCreate(
            startPmAsyncSampling,
            "Async PM sampling",
            1024,
            nullptr,
            1,
            nullptr
    );
}

void loop() {
    //check if the button has been pressed and handle appropriately
    if (makeDiscoverable) {
        server->startAdvertising();
        makeDiscoverable = false;
    }

    //obtain measurement from BMP280 Sensor
    sensor_data_t sample = ((BmpSensor *) bmpSensor)->sampleLowEnergy();
    server->setPressure(sample.pressure);
    server->setTemperature(sample.temperature);
    //Obtain measurement from PM Sensor
    sensor_data_t pmSample = pmSensor->sample();

    //Print measured data
    Serial.printf("Pressure: %f hPa | Temperature: %f °C | PM10: %f ppm | PM2.5: %f ppm\n",
                  sample.pressure,
                  sample.temperature,
                  pmSample.pm10,
                  pmSample.pm25);

    //wait for one measurement cycle
    delay(SLEEP_TIME * 1000);
}