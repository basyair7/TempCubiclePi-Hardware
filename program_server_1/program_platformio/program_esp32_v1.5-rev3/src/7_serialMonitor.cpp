/*! @file 7_serialMonitor.cpp
 * @version 1.5-rev3
*/

#include <Arduino.h>
#include <WiFi.h>
#include "1_configProgram.h"

unsigned long currentTimeMain = 0;
void print_data(void) {
  if(millis() - currentTimeMain >= interval_5) {
    Serial.println(F("\nControl Voltage Fan Exhaust & Heater"));
    Serial.println(F("******************************************************"));
    Serial.print(F("IP Address           : "));
    Serial.print((ipAddress != "" && WiFi.status() == WL_CONNECTED) || WiFi.getMode() == WIFI_AP ? "http://" + String(ipAddress) : ""); 
    Serial.println(F((ipAddress != "" && WiFi.status() == WL_CONNECTED) || WiFi.getMode() == WIFI_AP ? "/help" : "No WiFi Connection"));
    
    Serial.print(F("Temperature          : ")); Serial.print(datadht.temperature); Serial.println(F("*C"));
    Serial.print(F("Humidity             : ")); Serial.print(datadht.humidity); Serial.println(F("%"));
    Serial.print(F("Voltage Heater       : ")); Serial.print(datapzem.V); Serial.println(F(" V"));
    Serial.print(F("Current Heater       : ")); Serial.print(datapzem.I); Serial.println(F(" A"));
    Serial.print(F("Power Heater         : ")); Serial.print(datapzem.P); Serial.println(F(" W"));
    Serial.print(F("Energy Heater        : ")); Serial.print(datapzem.E); Serial.println(F(" Wh"));
    Serial.print(F("Frequency            : ")); Serial.print(datapzem.Freq); Serial.println(F(" Hz"));
    Serial.print(F("Pf Heater            : ")); Serial.print(datapzem.pF); Serial.println(F("%\n"));    
    Serial.print(F("Dimmer Heater        : ")); Serial.print(datafuzzy.powerHeater); Serial.println(F("%"));
    Serial.print(F("Fis Output Heater    : ")); Serial.println(datafuzzy.fisOutputHeater);
    Serial.print(F("Dimmer Fan           : ")); Serial.print(datafuzzy.powerFan); Serial.println(F("%"));
    Serial.print(F("Fis Output Fan       : ")); Serial.println(datafuzzy.fisOutputFan);
    Serial.println(F("******************************************************\n"));
    
    currentTimeMain = millis();
  }
}