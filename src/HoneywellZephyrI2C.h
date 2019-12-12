#ifndef __HONEYWELL_ZEPHYR_I2C_H__
#define __HONEYWELL_ZEPHYR_I2C_H__

#include <Wire.h>

/*!
 * @file HoneywellZephyrI2C.h
 *
 * @mainpage Honeywell Zephyr HAF digital flow rate sensor I2C driver
 *
 * @section intro_sec Introduction
 *
 * This is the documentation for the Hui Lab's Honeywell Zephyr 
 * HAF flow sensor driver for the Arduino platform.  It is designed based on
 * the <a href="https://sensing.honeywell.com/sensors/airflow-sensors/HAF-high-accuracy-50SCCM-750SCCM-series">
 * Honeywell technical note</a> for this product.
 
 * DEC.11/2019 added driver for SLPM models of Honeywell Zephyr 
 * a PDF datasheet was added to the main folder as a reference
 * @section dependencies Dependencies
 *
 * This library depends on the  <a href="https://www.arduino.cc/en/Reference/Wire">
 * Arduino Wire library</a>, included in a standard Arduino installation.
 *
 * @section author Author
 *
 * Written by Erik Werner for the Hui Lab.
 *
 * @section license License
 *
 * MIT license
 *
 */

/**************************************************************************/
/*! 
    @brief  Class for reading flow rate from a Honeywell Zephyr HAF sensor
*/
/**************************************************************************/
class ZephyrFlowRateSensor
{
    const uint8_t _ADDR;      ///< slave select pin (active low)
    const float _FLOW_RANGE;  ///< sensor flow rate range
    uint8_t _buf[2];          ///< buffer to hold sensor data
    int _count = 0;           ///< hold raw flow rate data (14- bits, 0 - 16384)

  public:
    /**************************************************************************/
    /*!
    @brief  Constructs a new flow rate sensor object.
    @param    address
              7-bit i2c address of the sensor
    @param    range
              the flow rate range of the sensor in CCM  for use with flow()
			  									or LPM	for use with flowLPM()
			   
    */
    /**************************************************************************/
    ZephyrFlowRateSensor(const uint8_t address, const float range)
        : _ADDR(address), _FLOW_RANGE(range) {}

    /**************************************************************************/
    /*!
    @brief  Initializes a pressure sensor object.
            This function must be called in the Arduino setup() function.
            Wire.begin() must be called seperately in setup() before the sensor
            can be used.
    */
    /**************************************************************************/
    void begin()
    {
        delay(17); // start-up time
        // power up sequence: first two reads are serial number
        readSensor(); // two MSB of Sensor
        
        delay(10);
        readSensor(); // 2 LSB of SN
    }

    /**************************************************************************/
    /*!
    @brief  Attempts to read two bytes from the sensor. The raw flow rate count 
    is updated.  Makes is no guanantee that the data retrieved from the 
    sensor is fresh data.
    @return 0 if two bytes were read, otherwise returns 1. 
    */
    /**************************************************************************/
    uint8_t readSensor()
    {
        Wire.requestFrom(_ADDR, (uint8_t)2);

        uint8_t idx = 0;
        while ( Wire.available() && idx < 2 ) {
            _buf[idx] = Wire.read();
            ++idx;
        }
        // data is MSB, LSB. First two bits always 00
        if(idx > 1) {
            _count = (uint16_t)_buf[1] & 0xFF;
            _count |= ( ( (uint16_t)_buf[0] ) << 8 ) & 0xFF00;
            return 0;
        }
        else {
            return 1;
        }
    }

    /**************************************************************************/
    /*!
    @brief  Read the most recently polled flow rate value.
        Update this value by calling readSensor() before reading.
    
    @return  The flow rate value from the most recent reading in raw counts
    */
    /**************************************************************************/
    int rawFlow() const { return _count; }


    /**************************************************************************/
    /*!
    @brief  Read the most recently polled flow rate value converted to SCCM
        Update this value by calling readSensor() before reading.
		Full Scale Flow refers to the SCCM of the device eg. 50 SCCM = 50
        Flow Applied = Full Scale Flow * [(Digital Output Code/16384) - 0.5]/0.4

    @return  The flow rate value from the most recent reading in SCCM
    */
    /**************************************************************************/
    float flow() const { return _FLOW_RANGE * ( ( (float)_count/16384.0) - 0.5)/0.4; }

    /**************************************************************************/
    /*!
    @brief  Read the most recently polled flow rate value converted to SLPM
        Update this value by calling readSensor() before reading.
		Full Scale Flow refers to the SLPM of the device eg. 100LPM = 100

        Flow Applied = Full Scale Flow * [(Digital Output Code/16384) - 0.1]/0.8

    @return  The flow rate value from the most recent reading in SLPM
    */
    /**************************************************************************/
	float flowLPM() const { return _FLOW_RANGE * ( ( (float)_count/16384) - 0.1)/.8; }
	
	
	
};

#endif // End __HONEYWELL_ZEPHYR_I2C_H__ include guard
