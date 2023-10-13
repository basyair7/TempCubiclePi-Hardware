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

// Insert version program
String version = "1.5-rev3";

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
  // run repeatedly funcMain() if program fuzzy ready
  while(true) {
    funcMain();
    if(WiFi.status() == WL_CONNECTED) server.handleClient();
  }
}

void setup() {
  // put your setup code here, to run once:
  // Initialize serial communication with a baudrate of 9600 bits per seconds (bps)
  EEPROM.begin(EEPROM_SIZE);
  Serial.begin(9600);
  atmegaSerial.begin(9600);
  // Initialize DHT-22 Sensor
  dht.begin();
  
  // Check fuzzy condition is disabled or not
  int stateFuzzySwitch = EEPROM.read(0);
  int stateBuzzerSwitch = EEPROM.read(1);
  if(stateFuzzySwitch == 0) stateFuzzy = false;
  if(stateBuzzerSwitch == 0) buzzerSwitch = false;

  // Turn on buzzer_startup if server esp is ready!
  buzzer_startup(pin_buzzer);
  delay(3000);
  
  // create a Task for program 1
  xTaskCreatePinnedToCore(
    program_1, "programMain", 5000, NULL, 1, NULL, CONFIG_ARDUINO_RUNNING_CORE
  );
  
  // create a Task for program 2
  xTaskCreatePinnedToCore(
    keepWiFiAlive, "WebServer", 5000, NULL, 2, NULL, CONFIG_ARDUINO_RUNNING_CORE
  );
  
}

void loop() {
  // NOTHING  
}