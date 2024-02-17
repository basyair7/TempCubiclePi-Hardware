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
uint64_t previous_time = 0;
uint8_t selfReset = 0;
// int changeMode = SelfChangeMode;

uint64_t currentTimeLedStart = 0, currentTimeLedEnd = 0;
uint8_t LedState = HIGH;
void ledState(uint8_t STATE) {
    digitalWrite(LED_BUILTIN, STATE);
}

void ledMode(bool Mode_wifi_AP, uint64_t interval_start, uint64_t interval_end) {
    if(!Mode_wifi_AP) return;
    if(LedState == HIGH) {
        if(millis() - currentTimeLedStart >= interval_start) {
            currentTimeLedStart = millis();
            LedState = LOW;
        }
    } else {
        if(millis() - currentTimeLedEnd >= interval_end) {
            currentTimeLedEnd = millis();
            LedState = HIGH;
        }
    }
    ledState(LedState);
}

void checkWiFiConfig() {
    Serial.print(F("SSID : "));
    Serial.println(loadSSID());
    Serial.print(F("Password : "));
    Serial.println(loadPassword());
}

void changeModeAP() {
    Serial.print(F("Self reset in : ")); Serial.println(10-selfReset);
    if(selfReset > 10) {
        saveConfig("changeMode", true);
        ESP.restart();
    }
    selfReset++;
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
    long current_time = millis();
    if((WiFi.status() != WL_CONNECTED && !wifi_AP_mode && loadSSID() != "" && loadPassword() != "") 
        && (current_time - previous_time >= interval_reconnect)) 
    {
        if(SelfChangeMode) changeModeAP();
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
            funcMain();
            ProgramPushButton();
        }
        ipAddress = WiFi.localIP().toString().c_str();
    } else {
        WiFi.softAP(STASSID, STAPSK);
        ipAddress = WiFi.softAPIP().toString().c_str();
    }
}