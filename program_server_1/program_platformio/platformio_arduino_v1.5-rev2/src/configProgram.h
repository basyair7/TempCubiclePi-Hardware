/*! @file configProgram.h
 * @version 1.5-rev2
*/

#include <Arduino.h>

#ifndef CONFIGPROGRAM_H
#define CONFIGPROGRAM_H

extern int getPower_ACFan, getPower_ACHeater;
extern int callback_2, callback_3;

// deklarasi variabel interval program berjalan
#define interval_1  50 // 50ms = 0.05 detik => lama waktu menerima data
#define interval_2  50 // 50ms = 0.05 detik => lama waktu mengirim data

// Pinout Dimmer Module
#define SignalPin_heater 5
#define SignalPin_fan    6

// Pinout LED Indikator
#define PINLED  LED_BUILTIN
extern int LedState;

// deklarasi fungsi program
// handleProgram.cpp
extern void ledInit(byte ledPin, int STATE);
extern void ledError(byte ledPin, long millisMain, long interval_start, long interval_end);
extern void atmegaReset(unsigned long millisMain, long interval, bool reset);

// SerialData.cpp
extern void getDataESP();
extern void sendSerialData(long millisMain, int powerHeater, int powerFan);
extern void sendCallback(void);

#endif