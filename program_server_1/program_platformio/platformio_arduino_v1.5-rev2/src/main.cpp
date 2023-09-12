/*! @file main.cpp
 * @version 1.5-rev2
 * @mainpage
 * Program proses fuzzifikasi modul dimmer (PlatformIO)
 * @section
 * VERSION : 1.5-rev2
 * NAMA    : FATHUL BASYAIR
 * NPM     : 1904105010004
 * PRODI   : TEKNIK ELEKTRO (BIDANG TEKNIK TENAGA LISTRIK)
*/

#include <Arduino.h>
#include <RBDdimmer.h>
#include "configProgram.h"

// dimmer module object
dimmerLamp dimmerACHeater(SignalPin_heater);
dimmerLamp dimmerACFan(SignalPin_fan);

// LED Indikator State
int LedState = HIGH;

// Callback Serial Data State
int callback_2 = 0, callback_3 = 0;

unsigned long waktuSebelum_1 = 0;

void setPowerDimmer(int setPowerHeater, int setPowerFan) {
  // Set output value: heater
  dimmerACHeater.setPower(setPowerHeater);
  // Set output value: fan
  dimmerACFan.setPower(setPowerFan);
  // Set output ledInit: HIGH
  ledInit(PINLED, HIGH);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  // initialize the DimmerAC pins for output : name.begin(mode, state);
  // Pin mode for Output: heater
  dimmerACHeater.begin(NORMAL_MODE, ON);
  dimmerACHeater.setPower(100);
  // Pin mode for Output: fan
  dimmerACFan.begin(NORMAL_MODE, ON);
  // Pin mode for Output: LED Indikator
  pinMode(PINLED, OUTPUT);
  ledInit(PINLED, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned long waktuSekarang = millis();
  if ((unsigned long) (waktuSekarang - waktuSebelum_1) >= interval_1)
  {
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