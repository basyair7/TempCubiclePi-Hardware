/* 
 * Program Pengambilan dan Mengirim data ESP32 ke server Raspberry Pi (Web Server Local)
 * Version : 1.5-rev2
 * NAMA    : FATHUL BASYAIR
 * NPM     : 1904105010004
 * PRODI   : TEKNIK ELEKTRO (BIDANG TEKNIK TENAGA LISTRIK)
 */
 
#include <DHT.h>
#include <Wire.h>
#include <WiFi.h>
#include <EEPROM.h>
#include <ESPmDNS.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <HTTPUpdateServer.h>
#include <ArduinoJson.h>
#include <PZEM004Tv30.h>
#include <SoftwareSerial.h>
#include "1_configProgram.h"

// Pinout Serial Data Arduino
#define atmegaRX1  25
#define atmegaTX1  26
SoftwareSerial atmegaSerial(atmegaRX1, atmegaTX1);

// Fuzzy Logic Object
DATAFUZZY datafuzzy;
FIS_TYPE g_fisInput[fis_gcI];
FIS_TYPE g_fisOutput[fis_gcO];

// Pinout Buzzer
#define pin_buzzer 22
int BuzzerState = HIGH;

// Pinout Sensor DHT
#define pinDHT     4
#define DHTTYPE   DHT21
DHT dht(pinDHT, DHTTYPE);
DATADHT datadht;

// Pinout Serial Data Sensor PZEM 004T
#define RXD2      16
#define TXD2      17
#if defined(ESP32)
PZEM004Tv30 pzem(Serial2, RXD2, TXD2);
#else
PZEM004Tv30 pzem(Serial2);
#endif
DATAPZEM datapzem;

// TCP server at port 80 will respond to HTTP requests
WebServer server (80);
String page;
const char* ssid = STASSID; const char* password = STAPSK;

void setup() {
  // put your setup code here, to run once:
  // Inisialisasikan serial komunikasi dengan baudrate 9600 bit per seconds (bps)
  EEPROM.begin(EEPROM_SIZE);
  Serial.begin(9600);
  atmegaSerial.begin(9600);

  int stateFuzzySwitch = EEPROM.read(0);
  if(stateFuzzySwitch == 0) stateFuzzy = false;

  // pasang konfigurasi WiFi ESP32 yang baru
  // cek reconnectWiFi jika wifi esp32 terputus dari server
  WiFi.onEvent(WiFiStationConnected, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_CONNECTED);
  WiFi.onEvent(WiFiGotIP, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_GOT_IP);
  WiFi.onEvent(WiFiStationDisconnected, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);
  
  // Inisialisasikan WiFi server ESP-32
  initWiFi();
  Serial.print(F("ESP32 NEW HOSTNAME : "));
  Serial.println(WiFi.getHostname());
  // Inisialisasikan web server ESP-32
  server_setup();

  // Inisialisasikan sensor dht
  dht.begin();
  // nyalakan buzzer_startup jika server esp telah ready!
  buzzer_startup(pin_buzzer);
  delay(5000);
}

void loop() {
  // put your main code here, to run repeatedly:
  funcMain();
  server.handleClient();
}
