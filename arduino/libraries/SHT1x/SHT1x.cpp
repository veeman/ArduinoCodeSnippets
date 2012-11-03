/**
 * SHT1x Library
 *
 * Copyright 2012 Vitalij Mast <vitalij-mast(at)mage-dev(dot).de>
 *
 * Manages communication with SHT1x series (SHT10, SHT11, SHT15)
 * temperature/humidity sensors from Sensirion (www.sensirion.com).
 *
 */

#include <Arduino.h>
#include "SHT1x.h"

SHT1x::SHT1x(int dataPin, int clockPin)
{
  _dataPin = dataPin;
  _clockPin = clockPin;
  lastValueType = vNone;
}

char SHT1x::getNextValue(float &cvalue, SHT1x::valType &type)
{
	char ret = 0;
	switch (lastValueType)
	{
		case vTemp:
			if (resultAvailable())
			{
				int val = getData16();
				cvalue = lastTemp = SHT_D1 + SHT_D2 * float(val);
				type = vTemp;
				ret = 1;
				lastValueType = vHumidity;
				sendCommand(CMD_HMD);
			}
			else
				ret = 0;
			break;
		case vHumidity:
			if (resultAvailable())
			{
				int val = getData16();
				float RHlin = SHT_C1 + SHT_C2 * float(val) + SHT_C3 * float(val) * float(val);
				
				cvalue = (lastTemp - 25.f)*(SHT_T1+SHT_T2* float(val))+ RHlin;
				type = vHumidity;
				ret = 1;
				lastValueType = vTemp;
				sendCommand(CMD_TMP);
			}
			else
				ret = 0;
			break;
		default:
			type = vNone;
			lastValueType = vTemp;
			ret = -sendCommand(CMD_TMP);
	}

	return ret;
}

float SHT1x::getLastTemp()
{
	return lastTemp;
}

char SHT1x::sendCommand(char command)
{
	pinMode(_dataPin, OUTPUT);
	pinMode(_clockPin, OUTPUT);

	// transmission Start
	digitalWrite(_dataPin, HIGH);
	  digitalWrite(_clockPin, HIGH);
	  digitalWrite(_dataPin, LOW);
		digitalWrite(_clockPin, LOW);
		digitalWrite(_clockPin, HIGH);
	digitalWrite(_dataPin, HIGH);
	digitalWrite(_clockPin, LOW);

	// send command (3 msb are address and must be 000, and last 5 bits are command)
	shiftOut(_dataPin, _clockPin, MSBFIRST, command & 0b00011111);

	// get ack
	digitalWrite(_clockPin, HIGH);
	pinMode(_dataPin, INPUT);
	char ret = !digitalRead(_dataPin);
	digitalWrite(_clockPin, LOW);

	return ret;
}

char SHT1x::resultAvailable()
{
   pinMode(_dataPin, INPUT);
   return !digitalRead(_dataPin);
}

int SHT1x::getData16()
{
	int val;

	// get the most significant bits
	pinMode(_dataPin, INPUT);
	pinMode(_clockPin, OUTPUT);
	val = shiftIn(_dataPin, _clockPin, 8);
	val *= 256;

	// send the required ack
	pinMode(_dataPin, OUTPUT);
		digitalWrite(_dataPin, HIGH);
		digitalWrite(_dataPin, LOW);
		digitalWrite(_clockPin, HIGH);
		digitalWrite(_clockPin, LOW);

	// get the least significant bits
	pinMode(_dataPin, INPUT);
	val |= shiftIn(_dataPin, _clockPin, 8);

	// send nack (wo don't need the following crc)
	pinMode(_dataPin, OUTPUT);
		digitalWrite(_dataPin, HIGH);
		digitalWrite(_clockPin, HIGH);
		digitalWrite(_clockPin, LOW);

	return val;
}