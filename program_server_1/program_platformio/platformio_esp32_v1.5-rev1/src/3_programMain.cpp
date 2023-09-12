/*! @file 3_programMain.cpp
 * @version 1.5-rev2
*/

#include <Arduino.h>
#include "1_configProgram.h"

// deklarasi variabel millis waktu sebelum jalannya program
unsigned long waktuSebelum_1 = 0, waktuSebelum_2 = 0;
unsigned long waktuSebelum_3 = 0, waktuSebelum_4 = 0;

void funcMain(void) {
    unsigned long waktuSekarang = millis();
    if((unsigned long) (waktuSekarang - waktuSebelum_1) >= interval_1) {
        waktuSebelum_1 = waktuSekarang;
        sendDataESP();
    }

    if((unsigned long) (waktuSekarang - waktuSebelum_2) >= interval_2) {
        waktuSebelum_2 = waktuSekarang;
        getDataAtmega();
    }

    if((unsigned long) (waktuSekarang - waktuSebelum_3) >= interval_3) {
        waktuSebelum_3 = waktuSekarang;
        // ambil data sensor DHT22 dan masukan ke program fuzzy
        datadht.humidity = humidity();
        datadht.temperature = temperature();
        g_fisInput[0] = datadht.temperature;
        g_fisInput[1] = datadht.humidity;
        g_fisOutput[0] = 0;
        g_fisOutput[1] = 0;
        fis_evaluate();
        datafuzzy.fisOutputHeater = g_fisOutput[0];
        datafuzzy.fisOutputFan = g_fisOutput[1];
    }

    if((unsigned long) (waktuSekarang - waktuSebelum_4) >= interval_4) {
        waktuSebelum_4 = waktuSekarang;
        // ambil data tegangan, arus, daya, energi, frekuensi, dan pf dari sensor pzem
        datapzem.V = readVoltage();
        datapzem.I = readCurrent();
        datapzem.P = readPower();
        datapzem.E = readEnergy();
        datapzem.Freq = readFreq();
        datapzem.pF = readpF();
        // jika temperature dibawah batas / tidak ada tegangan di jalur daya heater, maka buzzer akan menyala
        if(datadht.temperature < MIN_TEMP && datadht.humidity > MAX_HUM)
            buzzer_error(pin_buzzer, waktuSekarang, 200);
        if (datadht.temperature == 0.0 || datapzem.V == 0.0 || datapzem.I == 0.0) 
            buzzer_error(pin_buzzer, waktuSekarang, 200);
        else
            buzzer_main(pin_buzzer, LOW);
        
        // print_data();
    }
}

void funcTest(void) {
    Serial.println("Hello World!");
    delay(100);
}