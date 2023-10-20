/*! @file 9_buzzerMain.cpp
 * @version 1.5-rev3
 * @details
 * Connect a piezo buzzer or speaker to pin 11 or select a new pin.
 * More songs available at https://github.com/robsoncouto/arduino-songs                                            
 *                                            
 * Robson Couto, 2019
*/
#include <Arduino.h>
#include "pitches.h"
#include "1_configProgram.h"

// change this to make the song slower or faster
int tempo_1 = 280;
int tempo_2 = 108;

// notes of the moledy followed by the duration.
// a 4 means a quarter note, 8 an eighteenth , 16 sixteenth, so on
// !!negative numbers are used to represent dotted notes,
// so -4 means a dotted quarter note, that is, a quarter plus an eighteenth!!
int melody_1[] = {
  
  // Nokia Ringtone 
  // Score available at https://musescore.com/user/29944637/scores/5266155
  
  NOTE_E5, 8, NOTE_D5, 8, NOTE_FS4, 4, NOTE_GS4, 4, 
  NOTE_CS5, 8, NOTE_B4, 8, NOTE_D4, 4, NOTE_E4, 4, 
  NOTE_B4, 8, NOTE_A4, 8, NOTE_CS4, 4, NOTE_E4, 4,
  NOTE_A4, 2,   
};
int melody_2[] = {

  // The Lick 
  NOTE_D4,8, NOTE_E4,8, NOTE_F4,8, NOTE_G4,8, NOTE_E4,4, NOTE_C4,8, NOTE_D4,1,
  
};
int melody_3[] ={
  NOTE_C5, NOTE_G4, NOTE_G4, NOTE_A4, NOTE_G4, 0, NOTE_B4, NOTE_C5
};

// sizeof gives the number of bytes, each int value is composed of two bytes (16 bits)
// there are two values per note (pitch and duration), so for each note there are four bytes
int notes_1 = sizeof(melody_1) / sizeof(melody_1[0]) / 2;
int notes_2 = sizeof(melody_2) / sizeof(melody_2[0]) / 2;

// this calculates the duration of a whole note in ms
int wholenote_1 = (60000 * 4) / tempo_1;
int wholenote_2 = (60000 * 4) / tempo_2;

int divider_1 = 0, noteDuration_1 = 0, divider_2 = 0, noteDuration_2 = 0;
int noteDuration_3[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};

// this program tone buzzer in esp32
int playing = 0;
bool shutdown_sound = false;
unsigned long waktuSebelum_playBuzzer = 0;
void tone(byte pin, int freq, int d) {
  ledcSetup(0, 2000, 8); // setup beeper
  ledcAttachPin(pin, 0); // attach beeper
  ledcWriteTone(0, freq); // play tone
  playing = pin; // store pin
  vTaskDelay(d / portTICK_PERIOD_MS);
}
void noTone(byte pin) {
  tone(playing, 0, 0);
}

// program buzzer main
void buzzer_main(byte buzzerPin, int STATE) {
  if(!shutdown_sound){
    if(buzzerSwitch == true) {
      if (STATE == HIGH) tone(buzzerPin, 1000, 20);
      if (STATE == LOW) noTone(buzzerPin);
    } else {
      noTone(buzzerPin);
    }
  }
}

// program buzzer error
void buzzer_error(byte buzzerPin, long millisMain, long interval) 
{
  if(BuzzerState == HIGH) {
    if((unsigned long) (millisMain - waktuSebelum_playBuzzer) >= interval) {
      waktuSebelum_playBuzzer = millisMain;
      BuzzerState = LOW;
    }
  }
  else {
    if((unsigned long) (millisMain - waktuSebelum_playBuzzer) >= interval) {
      waktuSebelum_playBuzzer = millisMain;
      BuzzerState = HIGH;
    }
  }
  buzzer_main(buzzerPin, BuzzerState);
}

// program buzzer startup
void buzzer_startup(byte buzzer) {
  Serial.println("\nHello.. Welcome to TempCubiclePi Microcontroller\nVersion "+String(version));
  // iterate over the notes of the melody_1. 
  // Remember, the array is twice the number of notes (notes + durations)
  for (int thisNote = 0; thisNote < notes_1 * 2; thisNote = thisNote + 2) {

    // calculates the duration of each note
    divider_1 = melody_1[thisNote + 1];
    if (divider_1 > 0) {
      // regular note, just proceed
      noteDuration_1 = (wholenote_1) / divider_1;
    } else if (divider_1 < 0) {
      // dotted notes are represented with negative durations!!
      noteDuration_1 = (wholenote_1) / abs(divider_1);
      noteDuration_1 *= 1.5; // increases the duration in half for dotted notes
    }

    // we only play the note for 90% of the duration, leaving 10% as a pause
    tone(buzzer, melody_1[thisNote], noteDuration_1*0.9);

    // Wait for the specief duration before playing the next note.
    vTaskDelay(noteDuration_1 / portTICK_PERIOD_MS);
    
    // stop the waveform generation before the next note.
    noTone(buzzer);
  }
}

// program buzzer shutdown
void buzzer_shutdown(byte buzzer) {
  shutdown_sound = true;
  for (int thisNote = 0; thisNote < notes_2 * 2; thisNote = thisNote + 2) {
    // calculates the duration of each note
    divider_2 = melody_2[thisNote + 1];
    if (divider_2 > 0) {
      // regular note, just proceed
      noteDuration_2 = (wholenote_2) / divider_2;
    } else if (divider_2 < 0) {
      // dotted notes are represented with negative durations!!
      noteDuration_2 = (wholenote_2) / abs(divider_2);
      noteDuration_2 *= 1.5; // increases the duration in half for dotted note
    }

    // we only play the note for 90% of the duration, leaving 10% as a pause
    tone(buzzer, melody_2[thisNote], noteDuration_2*0.9);

    // Wait for the specief duration before playing the next note.
    vTaskDelay(noteDuration_2 / portTICK_PERIOD_MS);

    // Stop the wavefrom generation before the next note.
    noTone(buzzer);
  }
}

// Notification Sound
// Logic Fuzzy
void NotifFuzzy (uint8_t pinSound, bool state) {
  if(state == true) {
    tone(pinSound, NOTE_A4, 500);
    tone(pinSound, NOTE_D5, 200);
    noTone(pinSound);
  }
  if(state == false) {
    tone(pinSound, NOTE_D5, 500);
    tone(pinSound, NOTE_A4, 200);
    noTone(pinSound);
  }
}

// Buzzer Enable
void NotifBuzzer (uint8_t pinSound) {
  for (int thisNote = 0; thisNote < 8; thisNote++) {
    int noteDuration = 1000 / noteDuration_3[thisNote];
    tone(pinSound, melody_3[thisNote], noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
    vTaskDelay(pauseBetweenNotes / portTICK_PERIOD_MS);
    noTone(pinSound);
  }
}