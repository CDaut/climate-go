# Climate GO
Climate GO is a fully open source open hardware Sensor pack, to track and analyze personal air quality data. Its goal is to provide a way to monitor ones own air quality and motivate people to avoid polluted places and protect the environment.

## Paradigms
This project is intended to be open source, open hardware and open data oriented. It is of central importance to make this available to anyone who whishes to build a climate go device and to ensure no personal data is collected and stored. This is especially important because the climate data collected will be correlated with GPS information and could thus be used to construct a movement profile if not handled with uttermost care.

## Technical information
### Stack
The sensor pack itself is based around the ESP32 microprocessor and contains several sensors:
- BMP280 air pressure and temperature monitoring sensor
- Nova SDS011 particulate matter sensor
- [name] CO2 sensor
- MISC 2714 NO2 sensor

A custom PCB/housing is yet to be designed and manufactured. Data collection is made possible by an Android App, that will connect to the sensor pack via Bluetooth low energy. It is imperative to design the sensor pack to be lightweight and consume as little as possible energy to ensure comfort in transporting the sensor pack (it is intended to be clipped to e.g. a backpack) and long battery life.

## Outlook
It might be interesting to add more types of sensors, to collect more information and provide a broader picture of personal air quality. However this will increase cost and power consumption of the sensor and must thus be considered carefully.

It would also be interesting to "gamify" the idea, by providing users the opportunity to collect awards and complete quests related to personal air quality, increasing the motivation to protect the environment and b healthy even more.
