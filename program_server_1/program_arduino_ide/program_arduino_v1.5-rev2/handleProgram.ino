void (* resetFunc) (void) = 0; // declare reset function at address 0
int i = 0;

// create ledError function
void ledError(byte ledPin, long millisMain, long interval_start, long interval_end) {
  // set state with millis
  if(LedState == HIGH) {
    if((unsigned long) (millisMain - currentTimeLED_start) >= interval_start) {
      currentTimeLED_start = interval_start;
      LedState = LOW;
    }
  }
  else {
    if((unsigned long) (millisMain - currentTimeLED_end) >= interval_end) {
      currentTimeLED_end = interval_end;
      LedState = HIGH;
    }
  }
  ledInit(ledPin, LedState);
}

// create ledInit function
void ledInit(byte ledPin, int STATE) {
  digitalWrite(ledPin, STATE);
}

// create atmegaReset function
void atmegaReset(unsigned long millisMain, long interval, bool reset) {
  if(reset == false) i = 0; // if self reset disable, i = 0
  if((unsigned long) (millisMain - currentTimeReset) >= interval) {
    i++;
    if(i > 5) resetFunc(); // if error/i > 5 then run resetFunc();
  }
}