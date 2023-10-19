/*! @file 8_configWiFi.cpp
 * @version 1.5-rev3
*/

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <HTTPClient.h>
#include "1_configProgram.h"

// program reconnect wifi server
String ipAddress;
unsigned long previous_time = 0;
int selfReset = 0;
// int changeMode = SelfChangeMode;

void initWiFi()
{
    if(loadSSID() != "" && loadPassword() != "") {
        
        WiFi.mode(WIFI_STA);
        WiFi.begin(loadSSID(), loadPassword());
        Serial.println(F("Connecting to WiFi..."));
        while (WiFi.status() != WL_CONNECTED) {
            // Serial.print(F("."));
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
        ipAddress = WiFi.localIP().toString().c_str();
    } else {
        WiFi.softAP(ssid, password);
        ipAddress = WiFi.softAPIP().toString().c_str();
    }
}

// Reconnecting WiFi AP Server
void reconnectWiFi()
{
    unsigned long current_time = millis(); // number of milliseconds since the upload
    // checking for WIFI connection
    if ((WiFi.status() != WL_CONNECTED) && (current_time - previous_time >= interval_reconnect)) {
        Serial.print(current_time);
        Serial.println(F(" Reconnecting to WIFI network"));
        WiFi.disconnect();
        WiFi.reconnect();
        tone(pin_buzzer, 500, 100);
        tone(pin_buzzer, 2000, 20);
        noTone(pin_buzzer);
        previous_time = current_time;
    }
}