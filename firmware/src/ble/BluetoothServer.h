#ifndef NEW_CLIMTE_GO_BLUETOOTHSERVER_H
#define NEW_CLIMTE_GO_BLUETOOTHSERVER_H

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <Arduino.h>

#define temperatureCelsius

class BluetoothServer {
private:
    BLEService *sensorService;
    BLEServer *bleServer;
    bool advertiserLock;
public:
    BluetoothServer();

    void startAdvertising();
    void startServer();
    void setTemperature(float temperature);
    void setPressure(float pressure);
};


#endif //NEW_CLIMTE_GO_BLUETOOTHSERVER_H
