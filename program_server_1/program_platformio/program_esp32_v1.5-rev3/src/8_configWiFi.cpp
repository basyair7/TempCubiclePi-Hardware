/*! @file 8_configWiFi.cpp
 * @version 1.5-rev3
*/

#include <Arduino.h>
#include <WiFi.h>
#include "1_configProgram.h"

String ipAddress;

void keepWiFiAlive(void* param) {
    for(;;) {
        if(WiFi.status() == WL_CONNECTED) {
            // Serial.println(F("WiFi still connected"));
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            continue;
        }

        Serial.println(F("WiFi Connecting"));
        WiFi.mode(WIFI_STA);
        WiFi.begin(ssid, password);

        unsigned long startMillis = millis();
        while(WiFi.status() != WL_CONNECTED && 
            millis() - startMillis > interval_reconnect) {}

        if(WiFi.status() != WL_CONNECTED) {
            Serial.println(F("[WIFI] FAILED"));
            vTaskDelay(2000 / portTICK_PERIOD_MS);
            continue;
        }
        ipAddress = WiFi.localIP().toString().c_str();
        Serial.println("[WIFI] Connected" + String(ipAddress));
        server_setup();
    }
}