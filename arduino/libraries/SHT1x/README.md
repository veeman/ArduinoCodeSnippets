# Library Humidity Sensor sht1x
Copyright (c) 2012 Vitalij Mast <vitalij-mast(at)mage-dev(dot).de>
License see LICENSE file in the root folder
rev. 1.0

## Description
Unfortunately the interface and the protocoll of the SHT1x series (SHT10, SHT11, SHT15)
temperature/humidity sensors from [sensirion](http://www.sensirion.com) doesn't fit the
i2c/twi specification and you cann't use the hardware acceleration of the atmel µC directly.
This library manages the communication with these sensors. All data request are done 
asynchronously, so you can perform other commands while the sensor take a value. 

## Install
Just copy the whole folder `SHT1x` to your Arduino library directory (`arduino/libraries/`).
Restart the Arduino application and you will find an example project under `File/Examples/SHT1x`.

## Note
Thera are several Macro definition in the header file `SHT1x/SHT1x.h` to setup some settings
for the humidity sensor, like the bit resolution of the sensor. By default this set to 12 Bit
for humidity (SHT_RESRH_12) and 14 Bit for temperature (SHT_REST_14) and shouldn't be changed
because this is the init state of the ic.
The interesting Macro is the definition of the supply voltage in line 16. While some factors
depend on this value you must set this to your voltage. This option is by default set to 5V
(SHT_SUPPLY_5). Maybe you must adjust this to your value (available values are defined in the 
header).

## API Documentation

`enum SHT1x::valType {vNone, vTemp, vHumidity};`   
A definition of the value type returned.

`SHT1x::SHT1x(char dataPin, char clockPin);`   
This is the constructor of the interface, set the parameters to your pin numbers.

`char SHT1x::getNextValue(float &cvalue, SHT1x::valType &type);`   
This function trys to get the next alternate value either from the temperature or humidity 
sensor. On success the current value is stored on the parameter references.   
Return values: 0=no data available, <0=error, >0=data taken

`float SHT1x::getLastTemp();`   
Returns the last temperature value, on error this value returns zero.