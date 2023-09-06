#include <Arduino.h>
#include <DHT.h>
#include <PZEM004Tv30.h>
#include <SoftwareSerial.h>
#include <WebServer.h>

// setting find WiFi Hotspot
#ifndef STASSID
#define STASSID "Private"
#define STAPSK "Rumahmiruk_37"
extern const char* ssid;
extern const char* password;
extern String ipAddress;
#endif

#ifndef CONFIGPROGRAM_H
#define CONFIGPROGRAM_H

// informasi kubikel
#define kodekubikel "PA01"
extern String ipAddress;

// batas suhu normal dalam kubikel
#define MIN_TEMP 35 // 35 C
#define MAX_HUM  80 // 80 %

// pinout sensor
// Pinout Serial Data Arduino
#define atmegaRX1   25
#define atmegaTX1   26
extern int callback_1;
extern int callback_4;

// Pinout Buzzer
#define pin_buzzer  22
extern int BuzzerState;

// Pinout Serial Data Sensor PZEM-004T
#define RXD2        16
#define TXD2        17

// Pinout Sensor DHT
#define pinDHT      4

// EEPROM_SIZE Baudrate
#define EEPROM_SIZE 125

// deklarasi variabel interval program berjalan
#define interval_1 50   // 50ms = 0.05 detik => lama waktu mengirim data
#define interval_2 50   // 50ms = 0.05 detik => lama waktu menerima data
#define interval_3 2000 // 2000ms for program DHT
#define interval_4 1000 // 1000ms for program PZEM
#define interval_reconnect 15000 // 15 detik
#define interval_uploadData 1000 // 1 detik

// deklarasi fungsi program
// 3_programMain.cpp
extern void funcTest(void);
extern void funcMain(void);

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
extern void server_setup(void);

// 7_SerialMonitor.cpp
extern void print_data(void);

// 8_configWiFi.cpp
extern void initWiFi(void);
extern void WiFiStationConnected(WiFiEvent_t event, WiFiEventInfo_t info);
extern void WiFiGotIP(WiFiEvent_t event, WiFiEventInfo_t info);
extern void WiFiStationDisconnected(WiFiEvent_t event, WiFiEventInfo_t info);
extern void reconnectWiFi();

// 9_buzzerMain.cpp
extern void tone(byte pin, int freq, int d);
extern void noTone(byte pin);
extern void buzzer_main(byte buzzerPin, int STATE);
extern void buzzer_error(byte buzzerPin, long millisMain, long interval);
extern void buzzer_startup(byte buzzer);
extern void buzzer_shutdown(byte buzzer);

// 10_SerialData.cpp
extern void sendDataESP(void);
extern void getDataAtmega(void);

// 11_programFuzzy.cpp
extern void fis_evaluate();

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
extern String page;

//***********************************************************************
// Matlab .fis to arduino C converter v2.0.1.25122016                   
// - Karthik Nadig, USA                                                  
// Please report bugs to: karthiknadig@gmail.com                         
//***********************************************************************
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