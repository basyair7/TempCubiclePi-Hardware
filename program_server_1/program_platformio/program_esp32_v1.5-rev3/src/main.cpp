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
#include <Wire.h>
#include <WiFi.h>
#include <SPIFFS.h>
#include <WebServer.h>
#include <HTTPUpdateServer.h>
#include <PZEM004Tv30.h>
#include <SoftwareSerial.h>
#include "1_configProgram.h"

// Define a task handle and initialize it to NULL
TaskHandle_t taskHandle_1 = NULL;
TaskHandle_t taskHandle_2 = NULL;

// // Define static variables for tasks and task stacks
// StaticTask_t xTaskBuffer;
// StackType_t xStack[configMINIMAL_STACK_SIZE];

// Insert ssid and password server
const char* ssid = STASSID;
const char* password = STAPSK;

// Insert version program
String version = "1.5-rev3";

// TCP server at port 80 will respond to HTTP requests
WebServer server(80);
HTTPUpdateServer httpUpdater;
String page;
bool wifi_AP_mode = false;

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

// Push Button Mode
bool stateWiFiProgram = true;

// Serial Data Arduino Object
SoftwareSerial atmegaSerial(atmegaRX1, atmegaTX1);

// create a program 1 for the main program
void program_1(void* parameter) {
  (void) parameter;
  // run repeatedly funcMain() if program fuzzy ready
  while(true) {
    funcMain();
    ProgramPushButton();
    vTaskDelay(50 / portTICK_PERIOD_MS);
  }
}

// create a program 2 for the WiFi Connection program
void program_2(void* parameter) {
  (void) parameter;
  if(!wifi_AP_mode) {
    WiFi.onEvent(WiFiStationConnected, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_CONNECTED);
    WiFi.onEvent(WiFiGotIP, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_GOT_IP);
    WiFi.onEvent(WiFiStationDisconnected, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);
  }
  // initialize ESP32 WiFi Server
  initWiFi();
  Serial.print(F("ESP32 NEW HOSTNAME : "));
  Serial.println(WiFi.getHostname());
  server_setup();
  // run repeatedly server.handleClient() if the WiFi network is ready
  while(true) {
    if(wifi_AP_mode) ledMode(true, 1000, 1000);
    if(WiFi.status() == WL_CONNECTED || WiFi.getMode() == WIFI_AP) {
      server.handleClient();
    }
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void setup() {
  // put your setup code here, to run once:
  // Initialize serial communication with a baudrate of 9600 bits per seconds (bps)
  Serial.begin(9600);
  atmegaSerial.begin(9600);

  // initialize pinout button and LED
  setupProgramPushButton();
  
  // Initialize SPIFFS Program
  setupSPIFFS();
  
  // Initialize DHT-22 Sensor
  dht.begin();
  
  // Check fuzzy condition is disabled or not
  programReadSPIFFS();

  // Turn on buzzer_startup if server esp is ready!
  buzzer_startup(pin_buzzer);
  delay(3000);

  // create a Task for program 1
  xTaskCreatePinnedToCore(
    program_1, "MainProgram", 8192, NULL, 0, &taskHandle_1, 0
  );

  if(stateWiFiProgram)
    // create a Task for program 2
    xTaskCreatePinnedToCore(
      program_2, "WiFiProgram", 11290, NULL, 1, &taskHandle_2, 1
    );
}

void loop() {
  // NOTHING
}