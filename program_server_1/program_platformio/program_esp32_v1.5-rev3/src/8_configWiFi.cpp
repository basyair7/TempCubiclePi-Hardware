/*! @file 8_configWiFi.cpp
 * @version 1.5-rev3
*/

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <HTTPClient.h>
#include "1_configProgram.h"

// program reconnect wifi server
String ipAddress, ssid, password;
String APName, APPassword;
unsigned long previous_time = 0;
uint8_t selfReset = 0;
// int changeMode = SelfChangeMode;

unsigned long currentTimeLedStart = 0, currentTimeLedEnd = 0;
uint8_t LedState = HIGH;
void ledState(uint8_t STATE) {
    digitalWrite(LED_BUILTIN, STATE);
}

void ledMode(bool Mode_wifi_AP, unsigned long interval_start, unsigned long interval_end) {
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
    ssid = loadSSID(); password = loadPassword();
    Serial.print(F("SSID WiFi : "));
    Serial.println(ssid);
    Serial.print(F("Password WiFi : "));
    Serial.println(password);
    Serial.print(F("\nMode AP : "));
    Serial.println(
        (wifi_AP_mode == true ? "AP Hotspot" : "Client WiFi")
    );

    if(wifi_AP_mode){
        if(loadAPName() == "" || loadAPPassword() == "") {
            saveConfigAP(STASSID, STAPSK);
            APName = STASSID; APPassword = STAPSK;
            Serial.print(F("AP Name : "));
            Serial.println(APName);
            Serial.print(F("AP Password : "));
            Serial.println(APPassword);

        } else {
            APName = loadAPName(); APPassword = loadAPPassword();
            Serial.print(F("AP Name : "));
            Serial.println(APName);
            Serial.print(F("AP Password : "));
            Serial.println(APPassword);
        }
    }
}

void changeModeAP() {
    Serial.print(F("\nAuto Mode Access Point in : ")); Serial.println(10-selfReset);
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
    unsigned long current_time = millis();
    if((WiFi.status() != WL_CONNECTED && !wifi_AP_mode && ssid != "" && password != "") 
        && (current_time - previous_time >= interval_reconnect)) 
    {
        if(SelfChangeMode) changeModeAP();
        Serial.println(F("Disconnected from WiFi Access Point"));
        Serial.print(F("WiFi lost connection, Reason "));
        Serial.println(info.wifi_sta_disconnected.reason);
        Serial.println(F("Trying to reconnect"));
        WiFi.begin(ssid, password);
        tone(pin_buzzer, 500, 100);
        tone(pin_buzzer, 2000, 20);
        noTone(pin_buzzer);
        previous_time = current_time;
    }
}

void initWiFi()
{
    if(!wifi_AP_mode && ssid != "" && password != "") {
        WiFi.mode(WIFI_STA);
        WiFi.begin(ssid, password);
        Serial.println(F("Connecting to WiFi..."));
        while (WiFi.status() != WL_CONNECTED) {
            funcMain();
            ProgramPushButton();
        }
        ipAddress = WiFi.localIP().toString().c_str();
    } else {
        WiFi.softAP(APName, APPassword);
        ipAddress = WiFi.softAPIP().toString().c_str();
    }
}