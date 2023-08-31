/* 
 * Program proses fuzzifikasi modul dimmer
 * Version : 1.5-rev2
 * NAMA    : FATHUL BASYAIR
 * NPM     : 1904105010004
 * PRODI   : TEKNIK ELEKTRO (BIDANG TEKNIK TENAGA LISTRIK)
 */

#include "configProgram.h"
#include <RBDdimmer.h>
#include <ArduinoJson.h>

// Pinout Dimmer Module
#define SignalPin_heater  5
#define SignalPin_fan     6

dimmerLamp dimmerACHeater(SignalPin_heater);
dimmerLamp dimmerACFan(SignalPin_fan);

// Pinout LED Indikator
#define PINLED LED_BUILTIN
int LedState = HIGH;

// Setup routine runs once when you press reset:
void setup() {
  Serial.begin(9600);

  // initialize the DimmerAC pins for output : name.begin(mode, state);
  // Pin mode for Output: heater
  dimmerACHeater.begin(NORMAL_MODE, ON);
  dimmerACHeater.setPower(100);
  // Pin mode for Output: fan
  dimmerACFan.begin(NORMAL_MODE, ON);
  // Pin mode for Output : LED Indikator
  pinMode(PINLED, OUTPUT);
  ledInit(PINLED, HIGH);
}

// Loop routine runs over and over again forever:
void loop() {
  unsigned long waktuSekarang = millis();
  if ((unsigned long)(waktuSekarang - waktuSebelum_1) >= interval_1) {
    waktuSebelum_1 = waktuSekarang;
    sendCallback(); // run sendCallback function
    // Read Input: fisOutputHeater and fisOutputFan
    getDataESP();
    if(callback_2 == 1) {
      // disable reset atmega328p
      atmegaReset(waktuSekarang, 20000, false);
      setPowerDimmer(getPower_ACHeater, getPower_ACFan);
      // Send output value
      sendSerialData(waktuSekarang, dimmerACHeater.getPower(), dimmerACFan.getPower());
    } else {
      // disable led if callback none
      ledInit(PINLED, LOW);
      // self reset atmega328p if not callback
      atmegaReset(waktuSekarang, 20000, true);
    }
  }
}

void setPowerDimmer(int setPowerHeater, int setPowerFan) {
  // Set output value: voltage
  dimmerACHeater.setPower(setPowerHeater);
  // Set output value: fan
  dimmerACFan.setPower(setPowerFan);
  // Set output ledInit : HIGH
  ledInit(PINLED, HIGH);
}
