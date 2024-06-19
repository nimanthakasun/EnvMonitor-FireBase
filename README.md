# EnvMonitor-FireBase
Environmental monitor with Temperature Humidity measurements, Lux measurement, moisture measurement and drive a relay accordingly. This has a 16x2 LCD display to display information and send data to FireBase as well.
## Hardware I have used
- ESP32 Devkit v1
- Temperature Humidity Sensor - DHT22
- Soil Moisture sensor
- Lux sensor - TSL2561
## Description of Files
### EnvMonitor-FireBase.ino
This file is the main file of the project. setup and loop functions are included here
### FireBase_Util.ino
Contains all the firebase related functions
### NTP_Util.ino
Contains NTP time syncing related functions to support timestamp generation
### TSL2561_Util.ino
In this project I have used TSL2561 light sensor and this file contains lux level calculation functions by utilizing the TSL2561 sensor
### secrets.h
Contains your network details (WiFi Credentials) and Firebase Credentials
