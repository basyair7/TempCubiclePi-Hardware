/*! @file 1_configProgram.h
 * @version 1.5-rev3
*/

#include <Arduino.h>
#include <Wire.h>
#include <DHT.h>
#include <PZEM004Tv30.h>
#include <SoftwareSerial.h>
#include <WebServer.h>
#include <HTTPUpdateServer.h>

#ifndef CONFIGPROGRAM_H
#define CONFIGPROGRAM_H

// informasi firmware
#define FIRMWAREVERSION "1.5.3.13"
#define BUILDTIME       "18/2/2024 04:34 AM"
#define FIRMWAREREGION  "Indonesia"

// informasi kubikel
#define defaultkodekubikel "default"
extern String kodekubikel;

// setting find WiFi AP
#define STASSID "TempCubiclePi"
#define STAPSK  "87654321"
extern String ipAddress;
extern String ssid, password;
extern String APName, APPassword;
extern String version;

// batas suhu normal dalam kubikel
#define MIN_TEMP 35 // 35 C
#define MAX_HUM  80 // 80 %

// pinout sensor
// Pinout Push Button WiFi Mode
#define bootButton  0
#define ledProgramWiFiEnable LED_BUILTIN
extern bool stateWiFiProgram, SelfChangeMode;

// Pinout Sensor DHT
#define pinDHT      4

// Pinout Serial Data Sensor PZEM-004T
#define RXD2        16
#define TXD2        17

// Pinout Restart Hardware
#define GPIORESTART 18

// Pinout Buzzer
#define pin_buzzer  22
extern int BuzzerState;
extern bool buzzerSwitch;

// Pinout Serial Data Arduino
#define atmegaRX1   25
#define atmegaTX1   26
extern byte callback_1, callback_4;

// deklarasi variabel interval program berjalan
#define interval_1 50   // 50ms = 0.05 detik => lama waktu mengirim data
#define interval_2 50   // 50ms = 0.05 detik => lama waktu menerima data
#define interval_3 2500 // 2500ms for program DHT
#define interval_4 2000 // 2000ms for program PZEM
#define interval_5 5000 // 1000ms for program Serial Monitor
#define interval_reconnect 15000 // 15 detik
#define interval_uploadData 1000 // 1 detik

// deklarasi fungsi program
// main.cpp
extern void program_1(void* parameter);
extern void program_2(void* parameter);
// extern TaskHandle_t taskHandle_1;
// extern TaskHandle_t taskHandle_2;

// 3_programMain.cpp
extern void funcMain(void);
extern void ProgramPushButton(void);
extern void setupProgramPushButton(void);
extern void programReadSPIFFS(void);
extern void restartESP();

// 4_programPZEM.cpp
extern float readVoltage();
extern float readCurrent();
extern float readPower();
extern float readEnergy();
extern float readpF();
extern float readFreq();
extern void resetpzem(void);

// 5_programDHT.cpp
extern float temperature(void);
extern float humidity(void);

// 6_uploadData.cpp
extern bool mDNSESPReady;
extern void server_setup(void);

// 7_SerialMonitor.cpp
extern void print_data(void);

// 8_configWiFi.cpp
extern void ledMode(bool Mode_wifi_AP, unsigned long interval_start, unsigned long interval_end);
extern void initWiFi(void), checkWiFiConfig();
extern void WiFiStationConnected(WiFiEvent_t event, WiFiEventInfo_t info);
extern void WiFiGotIP(WiFiEvent_t event, WiFiEventInfo_t info);
extern void WiFiStationDisconnected(WiFiEvent_t event, WiFiEventInfo_t info);

// 9_buzzerMain.cpp
extern void tone(byte pin, int freq, int d);
extern void noTone(byte pin);
extern void NotifFuzzy (byte pinSound, bool state);
extern void NotifBuzzer (byte pinSound);
extern void buzzer_main(byte buzzerPin, int STATE);
extern void buzzer_error(byte buzzerPin, unsigned long millisMain, unsigned long interval);
extern void buzzer_startup(byte buzzer);
extern void buzzer_shutdown(byte buzzer);

// 10_SerialData.cpp
extern void sendDataESP(void);
extern void getDataAtmega(void);

// 11_programFuzzy.cpp
extern void fis_evaluate();

// 12_SPIFFSProgram.cpp
extern void setupSPIFFS(void);
extern bool readConfig(String stateConfig);
extern void saveConfig(String stateConfig, bool value);
extern void saveConfigWiFi(String ssid, String password);
extern void saveConfigAP(String ssid, String password);
extern void saveKubikelCode(String code);
extern void removeSPIFFS(String path);
extern void listSPIFFSFiles();
extern void openFileSPIFFS(const char* fileName);
extern String loadSSID(), loadAPName(), loadAPPassword();
extern String loadPassword(), loadKubikelCode();

// buat class DataFuzzy untuk menampung nilai dari arduino
class DATAFUZZY {
  public:
    float fisOutputHeater, fisOutputFan;
    int powerHeater, powerFan;
};

// buat class DataPzem untuk menampung nilai dari sensor PZEM
class DATAPZEM {
  public: 
    float V, I, P, E, Freq, pF;
};

// buat class DataDHT untuk menampung nilai dari sensor DHT
class DATADHT {
  public:
    float humidity, temperature;
};

// deklarasikan class object main
extern SoftwareSerial atmegaSerial;
extern DHT dht;
extern PZEM004Tv30 pzem;
extern DATAFUZZY datafuzzy;
extern DATADHT datadht;
extern DATAPZEM datapzem;
extern WebServer server;
extern HTTPUpdateServer httpUpdater;
extern String page;
extern bool wifi_AP_mode;

/*! @typedef 11_programFuzzy.cpp
 @details
 ***********************************************************************
 * Matlab .fis to arduino C converter v2.0.1.25122016                   
 * - Karthik Nadig, USA                                                  
 * Please report bugs to: karthiknadig@gmail.com                         
 ***********************************************************************
*/
#define FIS_TYPE float
#define FIS_RESOLUSION 101
#define FIS_MIN -3.4028235E+38
#define FIS_MAX 3.4028235E+38
typedef FIS_TYPE(*_FIS_MF)(FIS_TYPE, FIS_TYPE*);
typedef FIS_TYPE(*_FIS_ARR_OP)(FIS_TYPE, FIS_TYPE);
typedef FIS_TYPE(*_FIS_ARR)(FIS_TYPE*, int, _FIS_ARR_OP);

// Number of inputs to the fuzzy inference system
#define fis_gcI 2
// Number of outputs to the fuzzy inference system
#define fis_gcO 2
// Number of rules to the fuzzy inference system
#define fis_gcR 16

extern bool stateFuzzy;
extern FIS_TYPE g_fisInput[fis_gcI];
extern FIS_TYPE g_fisOutput[fis_gcO];

#endif