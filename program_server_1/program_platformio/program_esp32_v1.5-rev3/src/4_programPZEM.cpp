/*! @file 4_programPZEM.cpp
 * @version 1.5-rev3
*/

#include <Arduino.h>
#include "1_configProgram.h"

float readVoltage() {
  // mengambil data voltage dari sensor pzem
  float value = pzem.voltage();
  if (isnan(value)) {
    value = 0.0;
  }
  return value;
}

float readCurrent() {
  // mengambil data arus dari sensor pzem
  float value = pzem.current();
  if (isnan(value)) {
    value = 0.0;
  }
  return value;
}

float readPower() {
  // mengambil data daya dari sensor pzem
  float value = pzem.power();
  if (isnan(value)) {
    value = 0.0;
  }
  return value;  
}

float readEnergy() {
  // mengambil data energi terpakai dari sensor pzem
  float value = pzem.energy();
  if (isnan(value)) {
    value = 0.0;
  }
  return value;
}

float readpF() {
  // mengambil data pf dari sensor pzem
  float value = pzem.pf();
  if (isnan(value)) {
    value = 0.0;
  }
  return value;
}

float readFreq() {
  // mengambil data frekuensi dari sensor pzem
  float value = pzem.frequency();
  if (isnan(value)) {
    value = 0.0;
  }
  return value;
}

void resetpzem(void) {  
  pzem.resetEnergy();
}