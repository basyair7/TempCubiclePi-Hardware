/*! @file 5_programDHT.cpp 
 * @version 1.5-rev2
*/

#include <Arduino.h>
#include "1_configProgram.h"

// membuat program dht
float temperature(void)
{
    // mengambil nilai suhu
    float value = dht.readTemperature();
    // cek jika sensor bermasalah
    if (isnan(value))
    {
        // Serial.println(F("Failed to read temperature from DHT sensor!"));
        value = 0.0;
    }
    return value;
}

float humidity(void)
{
    // mengambil nilai kelembapan
    float value = dht.readHumidity();
    if (isnan(value)) 
    {
        // Serial.println(F("Failed to read humidity from DHT sensor!"));
        value = 0.0;
    }
    return value;
}