# Honeywell Zephyr I2C  [![Build Status](https://travis-ci.org/huilab/HoneywellZephyrI2C.svg?branch=master)](https://travis-ci.org/huilab/HoneywellZephyrI2C)
An Arduino library for communicating with Honeywell Zephyr HAF digital flow rate sensors over I2C.

Based on the Honeywell datasheets at: https://sensing.honeywell.com/sensors/airflow-sensors/HAF-high-accuracy-50SCCM-750SCCM-series. There is a also a copy of the datasheet in this repository.

Modified to work with the Honeywell SLPM Models of Zephyr Air Flow Sensor https://sensing.honeywell.com/sensors/airflow-sensors/HAF-high-accuracy-10SLPM-300SLPM
There is also a copy of the datasheet in this repository.

#### View the documentation
https://huilab.github.io/HoneywellZephyrI2C/

#### Install the library
To import the library manually, click the Download Zip button. In the Arduino IDE go to Sketch -> Include Library -> Add .ZIP Library.

Or, you may install this library with the Arduino Library Manager.

For details on installing libraries, see Arduino's guide at: https://www.arduino.cc/en/Guide/Libraries#toc3

#### Use
Include the library and declare a sensor:
```C
#include <HoneywellZephyrI2C.h>
// construct a 50 SCCM or SLPM sensor with address 0x49
// It does not matter if the sensor is SCCM or SLPM for this value (eg. 50), 
// for SCCM use sensor.flow()
// for SLPM use sensor.flowLPM()
ZephyrFlowRateSensor sensor( 0x49, 50 );
```
Initialize the sensor in `void setup()` :
```C
Wire.begin(); // start 2-wire communication
sensor.begin(); // run sensor initialization
```
Read values in `void loop()` :
```C
// the sensor returns 0 if new data is ready
if( sensor.readSensor() == 0 ) {
  float f = sensor.flow();
}
```

Read SLPM values in `void loop()` :
```C
// the sensor returns 0 if new data is ready
if( sensor.readSensor() == 0 ) {
  float f = sensor.flowLPM();
}
```


For more details, see the handy example sketch.
