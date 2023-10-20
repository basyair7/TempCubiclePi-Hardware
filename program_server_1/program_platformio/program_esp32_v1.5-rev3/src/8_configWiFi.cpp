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

unsigned long currentTimeLedStart = 0, currentTimeLedEnd = 0;
uint8_t LedState = HIGH;
void ledState(uint8_t STATE) {
    digitalWrite(LED_BUILTIN, STATE);
}

void ledMode(bool Mode_wifi_AP, long interval_start, long interval_end) {
    if(!Mode_wifi_AP) return;
    if(LedState == HIGH) {
        if((unsigned long) (millis() - currentTimeLedStart) >= interval_start) {
            currentTimeLedStart = millis();
            LedState = LOW;
        }
    } else {
        if((unsigned long) (millis() - currentTimeLedEnd) >= interval_end) {
            currentTimeLedEnd = millis();
            LedState = HIGH;
        }
    }
    ledState(LedState);
}

// Reconnecting WiFi AP Server
void WiFiStationConnected(WiFiEvent_t event, WiFiEventInfo_t info)
{
    Serial.println(F("Connection to AP Successfully"));
    selfReset = 0;
}

void WiFiGotIP(WiFiEvent_t event, WiFiEventInfo_t info) 
{
    ipAddress = WiFi.localIP().toString().c_str();
    Serial.println(F("WiFi Connected"));
    Serial.print(F("IP Address : "));
    Serial.println(ipAddress);
}

void WiFiStationDisconnected(WiFiEvent_t event, WiFiEventInfo_t info)
{
    unsigned long current_time = millis();
    if((WiFi.status() != WL_CONNECTED && !wifi_AP_mode && loadSSID() != "" && loadPassword() != "") 
        && ((unsigned long) (current_time - previous_time) >= interval_reconnect)) 
    {
        Serial.println(F("Disconnected from WiFi Access Point"));
        Serial.print(F("WiFi lost connection, Reason "));
        Serial.println(info.wifi_sta_disconnected.reason);
        Serial.println(F("Trying to reconnect"));
        WiFi.begin(loadSSID(), loadPassword());
        tone(pin_buzzer, 500, 100);
        tone(pin_buzzer, 2000, 20);
        noTone(pin_buzzer);
        previous_time = current_time;
    }
}

void initWiFi()
{
    if(!wifi_AP_mode && loadSSID() != "" && loadPassword() != "") {
        
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