#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLE2902.h>
#include "BluetoothServer.h"

#define INTERNAL_LED_PIN 2
#define BLE_SERVER_NAME "ClimateGO"
#define SERVICE_UUID "2150123c-af53-4038-bc92-ba3d0870a9e4"
#define TEMPERATURE_CHARACTERISTIC_UUID "cba1d466-344c-4be3-ab3f-189f80dd7518"
#define PRESSURE_CHARACTERISTIC_UUID "ca73b3ba-39f6-4ab3-91ae-186dc9577d99"
#define BLINK_TIME_DELAY_MS 500
#define BLINK_TIMES 5

//Setup callbacks onConnect and onDisconnect
class ServerCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer *pServer) override {
        Serial.println("New device connected.");
        digitalWrite(INTERNAL_LED_PIN, HIGH);
    };

    void onDisconnect(BLEServer *pServer) override {
        Serial.println("Device disconnected.");
        digitalWrite(INTERNAL_LED_PIN, LOW);
        //restart advertising on device disconnect
        pServer->getAdvertising()->start();
    }
};

/**
 * Represents the Bluetooth server that handles all interactions
 */
BluetoothServer::BluetoothServer() {
    // Create the BLE Device
    BLEDevice::init(BLE_SERVER_NAME);

    // Create the BLE Server
    this->bleServer = BLEDevice::createServer();
    bleServer->setCallbacks(new ServerCallbacks());

    // Create the BLE Service
    this->sensorService = bleServer->createService(SERVICE_UUID);

    //create characteristics that will enable data sharing
    BLECharacteristic *temperatureCharacteristic = sensorService->createCharacteristic(
            TEMPERATURE_CHARACTERISTIC_UUID,
            BLECharacteristic::PROPERTY_READ
    );
    BLECharacteristic *pressureCharacteristic = sensorService->createCharacteristic(
            PRESSURE_CHARACTERISTIC_UUID,
            BLECharacteristic::PROPERTY_READ
    );

    //set initial values
    temperatureCharacteristic->setValue("NA");
    pressureCharacteristic->setValue("NA");

    //allow advertiser function to be called
    this->advertiserLock = false;
}

/**
 * Method to start the Bluetooth server and enable advertising to allow other devices to connect
 */
void BluetoothServer::startServer() {
    // Start the service
    this->sensorService->start();

    // Start advertising
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    this->bleServer->getAdvertising()->start();
    Serial.println("Started BLE Server.");
}

/**
 * TODO: Convert temperature to string somehow
 * Publish a new temperature to the appropriate characteristic
 * @param temperature the new temperature
 */
void BluetoothServer::setTemperature(float temperature) {
    sensorService
            ->getCharacteristic(TEMPERATURE_CHARACTERISTIC_UUID)
            ->setValue(temperature);
}

/**
 * TODO: Convert pressure to string somehow
 * Publish a new pressure to the appropriate characteristic
 * @param pressure the new pressure
 */
void BluetoothServer::setPressure(float pressure) {
    sensorService
            ->getCharacteristic(PRESSURE_CHARACTERISTIC_UUID)
            ->setValue(pressure);
}

/**
 * This method will start advertising via Bluetooth and blink the LED
 */
void BluetoothServer::startAdvertising() {
    if (!this->advertiserLock) {
        this->advertiserLock = true;
        this->bleServer->getAdvertising()->start();
        Serial.println("Starting Bluetooth advertising...");
        for (int i = 0; i < BLINK_TIMES; ++i) {
            digitalWrite(INTERNAL_LED_PIN, HIGH);
            delay(BLINK_TIME_DELAY_MS);
            digitalWrite(INTERNAL_LED_PIN, LOW);
            delay(BLINK_TIME_DELAY_MS);
        }
        this->advertiserLock = false;
    }
}


