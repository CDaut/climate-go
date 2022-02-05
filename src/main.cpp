#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_BMP280.h>

Adafruit_BMP280 bmp; // use I2C interface
Adafruit_Sensor *bmp_temp = bmp.getTemperatureSensor();
Adafruit_Sensor *bmp_pressure = bmp.getPressureSensor();

void setup() {
    Serial.begin(112500);
    while ( !Serial ) delay(100);   // wait for native usb

    unsigned status;
    status = bmp.begin(0x76); //set the correct I2C port

    //query status and reboot the board if no sensor is detected
    if (!status) {
        ESP.restart();
    }

    /* Default settings from datasheet. */
    bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                    Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                    Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                    Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                    Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */

    bmp_temp->printSensorDetails();
}

void loop() {
    sensors_event_t temp_event, pressure_event;
    bmp_temp->getEvent(&temp_event);
    bmp_pressure->getEvent(&pressure_event);

    Serial.print(F("Temperature = "));
    Serial.print(temp_event.temperature);
    Serial.print(" *C | ");

    Serial.print(F("Pressure = "));
    Serial.print(pressure_event.pressure);
    Serial.println(" hPa");
    delay(100);
}