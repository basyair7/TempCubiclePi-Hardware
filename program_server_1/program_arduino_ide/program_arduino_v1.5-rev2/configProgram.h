/*! @file configProgram.h
 * @version 1.5-rev2
*/

#ifndef CONFIGPROGRAM_H
#define CONFIGPROGRAM_H

int getPower_ACFan, getPower_ACHeater;
int callback_2 = 0;
int callback_3 = 0;

// deklarasi variabel interval program berjalan
const long interval_1 = 50; // 50ms = 0.05 detik => lama waktu menerima data
const long interval_2 = 50; // 50ms = 0.05 detik => lama waktu mengirim data

// deklarasi variabel millis waktu sebelum jalannya program
unsigned long waktuSebelum_1 = 0;
unsigned long waktuSebelum_2 = 0;
unsigned long currentTimeReset = 0;
unsigned long currentTimeLED_start = 0;
unsigned long currentTimeLED_end = 0;

#endif