/*! @file 1_configProgram.h
 * @version 1.5-rev2
*/

// setting find WiFi Hotspot
String ipAddress;
#ifndef STASSID
#define STASSID "@Wifi.com"
#define STAPSK "Hostpot_ahul7"
#endif

#ifndef CONFIGPROGRAM_H
#define CONFIGPROGRAM_H

// informasi kubikel
#define kodekubikel "PA01"

// batas suhu normal dalam kubikel
#define MIN_TEMP 35 // 35 C
#define MAX_HUM  80 // 80 %

#define EEPROM_SIZE 125
int callback_1 = 0;
int callback_4 = 0;
int selfReset = 0;
bool stateFuzzy = true;
bool buzzerSwitch = true;

// deklarasi variabel interval program berjalan
const long interval_1 = 50;   // 50ms = 0.05 detik => lama waktu mengirim data
const long interval_2 = 50;   // 50ms = 0.05 detik => lama waktu menerima data
const long interval_3 = 2000; // 2000ms for program DHT
const long interval_4 = 1000; // 1000ms for program PZEM
const long interval_reconnect = 15000; // 15 detik
const long interval_uploadData = 1000; // 1 detik

// deklarasi variabel millis waktu sebelum jalannya program
unsigned long waktuSebelum_1 = 0;
unsigned long waktuSebelum_2 = 0;
unsigned long waktuSebelum_3 = 0;
unsigned long waktuSebelum_4 = 0;
unsigned long waktuSebelum_playBuzzer = 0;
unsigned long waktuSebelum_uploadData = 0;
unsigned long previous_time = 0;

// buat class DataFuzzy untuk menampung nilai dari arduino
class DATAFUZZY {
  public:
    float fisOutputHeater, fisOutputFan;
    int powerHeater, powerFan;
};

// buat class DataPzem untuk menampung nilai dari sensor PZEM
class DATAPZEM {
  public: 
    float V, I, P, E, Freq, pF;
};

// buat class DataDHT untuk menampung nilai dari sensor DHT
class DATADHT {
  public:
    float humidity, temperature;
};

/*! @typedef 11_programFuzzy.ino
 * @details
 ***********************************************************************
 * Matlab .fis to arduino C converter v2.0.1.25122016                   
 * - Karthik Nadig, USA                                                  
 * Please report bugs to: karthiknadig@gmail.com                         
 ***********************************************************************
*/

#define FIS_TYPE float
#define FIS_RESOLUSION 101
#define FIS_MIN -3.4028235E+38
#define FIS_MAX 3.4028235E+38
typedef FIS_TYPE(*_FIS_MF)(FIS_TYPE, FIS_TYPE*);
typedef FIS_TYPE(*_FIS_ARR_OP)(FIS_TYPE, FIS_TYPE);
typedef FIS_TYPE(*_FIS_ARR)(FIS_TYPE*, int, _FIS_ARR_OP);

// Number of inputs to the fuzzy inference system
const int fis_gcI = 2;
// Number of outputs to the fuzzy inference system
const int fis_gcO = 2;
// Number of rules to the fuzzy inference system
const int fis_gcR = 16;

#endif