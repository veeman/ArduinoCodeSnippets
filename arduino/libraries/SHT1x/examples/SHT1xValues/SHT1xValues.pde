/**
 * SHT1xValues
 *
 * Copyright 2012 Vitalij Mast <vitali-mast(at)mage-dev(dot).de>
 *
 * Manages communication with SHT1x series (SHT10, SHT11, SHT15)
 * temperature/humidity sensors from Sensirion (www.sensirion.com).
 *
 */
#include <SHT1x.h>

#define SHT_DATA  10
#define SHT_CLOCK 11

SHT1x sht1x(SHT_DATA, SHT_CLOCK);

void setup()
{
  Serial.begin(38400);
}

void loop()
{
  SHT1x::valType type;
  float humidity;
  if (sht1x.getNextValue(humidity, type) > 0)
  {
    if (type == SHT1x::vHumidity)
    {
      Serial.print("temperature: ");
      Serial.print(sht1x.getLastTemp(),2);
      Serial.print("°C\tpressure: ");
      Serial.print(humidity,2);
      Serial.println("%");
    }
  }
}