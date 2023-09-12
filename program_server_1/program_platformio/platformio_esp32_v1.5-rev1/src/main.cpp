/*! @file main.cpp
 * @version 1.5-rev2
 * @mainpage
 * Program Pengambilan dan Mengirim data ESP32 ke server Raspberry Pi (Web Server Local) (PlatformIO)
 * @section
 * Version : 1.5-rev2
 * NAMA    : FATHUL BASYAIR
 * NPM     : 1904105010004
 * PRODI   : TEKNIK ELEKTRO (BIDANG TEKNIK TENAGA LISTRIK)
*/

#include <Arduino.h>
#include <DHT.h>
#include <Wire.h>
#include <WiFi.h>
#include <EEPROM.h>
#include <WebServer.h>
#include <PZEM004Tv30.h>
#include <SoftwareSerial.h>
#include "1_configProgram.h"

// Insert ssid and password server
const char* ssid = STASSID;
const char* password = STAPSK;

// TCP server at port 80 will respond to HTTP requests
WebServer server(80);
String page;

// Fuzzy Logic Object
DATAFUZZY datafuzzy;
FIS_TYPE g_fisInput[fis_gcI];
FIS_TYPE g_fisOutput[fis_gcO];

// stateFuzzy enable or disable
// default : true (enable)
bool stateFuzzy = true;

// BuzzerState Main
int BuzzerState = HIGH;
bool buzzerSwitch = true;

// Sensor PZEM-004T Object
#if defined(ESP32)
PZEM004Tv30 pzem(Serial2, RXD2, TXD2);
#else
PZEM004Tv30 pzem(Serial2);
#endif
DATAPZEM datapzem;

// Sensor DHT Object
DHT dht(pinDHT, DHTTYPE);
DATADHT datadht;

// Serial Data Arduino Object
SoftwareSerial atmegaSerial(atmegaRX1, atmegaTX1);

void setup() {
  // put your setup code here, to run once:
  // Inisialisasikan serial komunikasi dengan baudrate 9600 bit per seconds (bps)
  EEPROM.begin(EEPROM_SIZE);
  Serial.begin(9600);
  atmegaSerial.begin(9600);
  
  // Cek kondisi fuzzy dinonaktifkan atau tidak
  int stateFuzzySwitch = EEPROM.read(0);
  int stateBuzzerSwitch = EEPROM.read(1);
  if(stateFuzzySwitch == 0) stateFuzzy = false;
  if(stateBuzzerSwitch == 0) buzzerSwitch = false;
  
  // pasang konfigurasi WiFi ESP32 yang baru
  // cek reconnectWiFi jika wifi esp32 terputus
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