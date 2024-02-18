/*! @file 3_programMain.cpp
 * @version 1.5-rev3
 */

#include <Arduino.h>
#include <WiFi.h>
#include "1_configProgram.h"

// deklarasi variabel millis waktu sebelum jalannya program
unsigned long waktuSebelum_1 = 0, waktuSebelum_2 = 0, waktuSebelum_3 = 0, waktuSebelum_4 = 0;
unsigned long lastPushButton = 0;
uint8_t count = 0, PushButtonDelay = 50;
String kodekubikel;
bool SelfChangeMode = false;

void funcMain(void)
{
    unsigned long waktuSekarang = millis();
    if (waktuSekarang - waktuSebelum_1 >= interval_1)
    {
        waktuSebelum_1 = waktuSekarang;
        sendDataESP();
    }

    if (waktuSekarang - waktuSebelum_2 >= interval_2)
    {
        waktuSebelum_2 = waktuSekarang;
        getDataAtmega();
    }

    if (waktuSekarang - waktuSebelum_3 >= interval_3)
    {
        waktuSebelum_3 = waktuSekarang;
        // ambil data sensor DHT22 dan masukan ke program fuzzy
        datadht.humidity = humidity();
        datadht.temperature = temperature();
        g_fisInput[0] = datadht.temperature;
        g_fisInput[1] = datadht.humidity;
        g_fisOutput[0] = 0;
        g_fisOutput[1] = 0;
        fis_evaluate();
        datafuzzy.fisOutputHeater = g_fisOutput[0];
        datafuzzy.fisOutputFan = g_fisOutput[1];
    }

    if (waktuSekarang - waktuSebelum_4 >= interval_4)
    {
        waktuSebelum_4 = waktuSekarang;
        // ambil data tegangan, arus, daya, energi, frekuensi, dan pf dari sensor pzem
        datapzem.V = readVoltage();
        datapzem.I = readCurrent();
        datapzem.P = readPower();
        datapzem.E = readEnergy();
        datapzem.Freq = readFreq();
        datapzem.pF = readpF();

        // jika temperature dibawah batas / tidak ada tegangan di jalur daya heater, maka buzzer akan menyala
        if (datadht.temperature < MIN_TEMP && datadht.humidity > MAX_HUM)
            buzzer_error(pin_buzzer, waktuSekarang, 500);

        if (datadht.humidity == 0.0 || datapzem.I == 0.0 || (datapzem.V == 0.0 && datapzem.I == 0.0))
            buzzer_error(pin_buzzer, waktuSekarang, 500);

        else
            buzzer_main(pin_buzzer, LOW);

        if (!stateWiFiProgram)
        {
            print_data();
        }
        if ((WiFi.status() == WL_CONNECTED && stateWiFiProgram && mDNSESPReady) || WiFi.getMode() == WIFI_AP)
        {
            print_data();
        }

        if (count > 5 && !stateWiFiProgram)
        {
            Serial.println(F("Mode WiFi di aktifkan..."));
            saveConfig("wifi", true);
            restartESP();
        }

        if (count > 5 && stateWiFiProgram)
        {
            Serial.println(F("Mode WiFi di nonaktifkan..."));
            saveConfig("wifi", false);
            restartESP();
        }
    }
}

void ProgramPushButton(void)
{
    int buttonState = digitalRead(bootButton);
    if (buttonState == LOW)
    {
        if (millis() - lastPushButton >= PushButtonDelay)
        {
            count++;
            lastPushButton = millis();
        }
    }
}

void setupProgramPushButton(void)
{
    pinMode(bootButton, INPUT_PULLUP);
    pinMode(ledProgramWiFiEnable, OUTPUT);
}

void programReadSPIFFS(void)
{
    if(loadKubikelCode() == "") saveKubikelCode(defaultkodekubikel);
    kodekubikel = loadKubikelCode();
    stateFuzzy = readConfig("fuzzy");
    buzzerSwitch = readConfig("buzzer");
    stateWiFiProgram = readConfig("wifi");
    wifi_AP_mode = readConfig("changeMode");
    SelfChangeMode = readConfig("autoChangeMode");
    
    digitalWrite(ledProgramWiFiEnable, stateWiFiProgram);
}

void restartESP(void) {
    buzzer_shutdown(pin_buzzer);
    delay(1000);
    esp_restart();
}