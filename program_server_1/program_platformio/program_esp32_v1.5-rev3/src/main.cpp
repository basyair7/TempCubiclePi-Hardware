/*! @file main.cpp
 * @version 1.5-rev3
 * @mainpage
 * Program Pengambilan dan Mengirim data ESP32 ke server Raspberry Pi (Web Server Local) (PlatformIO)
 * @section
 * VERSION : 1.5-rev3
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
#define DHTTYPE   DHT21
DHT dht(pinDHT, DHTTYPE);
DATADHT datadht;

// Serial Data Arduino Object
SoftwareSerial atmegaSerial(atmegaRX1, atmegaTX1);

// create a program 1 for WebServer
void program_1(void* parameter) {
  // Initialize DHT-22 Sensor
  dht.begin();
  // run repeatedly funcMain() if program fuzzy ready
  while(true) {
    funcMain();
  }
}

// create a program 2 for WebServer
void program_2(void* parameter) {
  // Initialize ESP-32 WiFi Server
  initWiFi();
  Serial.print(F("ESP32 NEW HOSTNAME : "));
  Serial.println(WiFi.getHostname());
  server_setup();
  // run repeatedly server.handleClient() if WiFi connected
  while(true) {
    server.handleClient();
  }
}

void setup() {
  // put your setup code here, to run once:
  // Initialize serial communication with a baudrate of 9600 bits per seconds (bps)
  EEPROM.begin(EEPROM_SIZE);
  Serial.begin(9600);
  atmegaSerial.begin(9600);
  
  // Check fuzzy condition is disabled or not
  int stateFuzzySwitch = EEPROM.read(0);
  int stateBuzzerSwitch = EEPROM.read(1);
  if(stateFuzzySwitch == 0) stateFuzzy = false;
  if(stateBuzzerSwitch == 0) buzzerSwitch = false;

  // Turn on buzzer_startup if server esp is ready!
  buzzer_startup(pin_buzzer);
  delay(5000);

  // Install the new ESP32 WiFi configuration
  // check reconnectWiFi if esp32 wifi is disconnected
  WiFi.onEvent(WiFiStationConnected, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_CONNECTED);
  WiFi.onEvent(WiFiGotIP, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_GOT_IP);
  WiFi.onEvent(WiFiStationDisconnected, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);
  
  // create a Task for program 1
  xTaskCreatePinnedToCore(
    program_1, "programMain", 5000, NULL, 1, NULL, ARDUINO_RUNNING_CORE
  );
  
  // create a Task for program 2
  xTaskCreatePinnedToCore(
    program_2, "programWebServer", 5000, NULL, 2, NULL, ARDUINO_RUNNING_CORE
  );
  
}

void loop() {
  // NOTHING  
}