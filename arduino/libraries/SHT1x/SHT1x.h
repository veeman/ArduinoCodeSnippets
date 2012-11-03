/**
 * SHT1x Library
 *
 * Copyright 2012 Vitalij Mast <vitalij-mast(at)mage-dev(dot).de>
 *
 * Manages communication with SHT1x series (SHT10, SHT11, SHT15)
 * temperature/humidity sensors from Sensirion (www.sensirion.com).
 *
 */
#ifndef SHT1x_h
#define SHT1x_h

#define CMD_TMP 0b00000011
#define CMD_HMD 0b00000101

#define SHT_SUPPLY_5
#define SHT_RESRH_12
#define SHT_REST_14

#if defined(SHT_RESRH_12)
  #define SHT_C1 -2.0468f
  #define SHT_C2 0.0367f
  #define SHT_C3 -1.5955E-6f
  #define SHT_T1 0.01f
  #define SHT_T2 0.00008f
#elif defined(SHT_RESRH_8)
  #define SHT_C1 -2.0468f
  #define SHT_C2 0.5872f
  #define SHT_C3 -4.0845E-4f
  #define SHT_T1 0.01f
  #define SHT_T2 0.00128f
#else
  #error "SHT1x unknown or unsupported humidity resoltion, please define SHT_RESRH_??. (??=Bit)"
#endif

#if defined(SHT_REST_14)
  #define SHT_D2 0.01f
#elif defined(SHT_REST_12)
  #define SHT_D2 0.04f
#else
  #error "SHT1x unknown or unsupported temperature resoltion, please define SHT_REST_??. (??=Bit)"
#endif

#if defined(SHT_SUPPLY_5)
  #define SHT_D1 -40.1f
#elif defined(SHT_SUPPLY_4)
  #define SHT_D1 -39.8f
#elif defined(SHT_SUPPLY_35)
  #define SHT_D1 -39.7f
#elif defined(SHT_SUPPLY_3)
  #define SHT_D1 -39.6f
#elif defined(SHT_SUPPLY_25)
  #define SHT_D1 -39.4f
#else
  #error "SHT1x unknown or unsupported supply volltage, please define SHT_SUPPLY_??. (??=Voltage)"
#endif

class SHT1x
{
  public:
	enum valType 
	{
		vNone,
		vTemp,
		vHumidity
	};

    SHT1x(char dataPin, char clockPin);
	
	char getNextValue(float &cvalue, SHT1x::valType &type);
	float getLastTemp();
  private:
    char _dataPin;
    char _clockPin;
	float lastTemp;
	valType lastValueType;

  private:
	char resultAvailable();
	char sendCommand(char command);
	int getData16();
};

#endif
