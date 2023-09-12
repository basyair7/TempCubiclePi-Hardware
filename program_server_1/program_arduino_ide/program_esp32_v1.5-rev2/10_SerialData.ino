/*! @file 10_SerialData.ino
 * @version 1.5-rev2
*/

void sendDataESP(void) {
  // buat object DynamicJsonDocument data
  DynamicJsonDocument data(500);
  // ambil command callback
  deserializeJson(data, atmegaSerial);
  callback_1 = data["callback_1"];
  if(callback_1 == 1) {
    // masukan nilai ke dalam format json
    // jika suhu pada MCU tidak terdeteksi, maka nilai FIS Heater 100
    if(datadht.temperature == 0.0 || datadht.temperature <= 0.0 || stateFuzzy == false) {
      data["heater"] = 100;
      data["fan"] = 0;
      data["callback_2"] = 1;
      data["callback_3"] = 1;
    } else {
      if(datadht.temperature >= 50.01 || datadht.humidity <= 30.0) {
        data["heater"] = 60;
        data["fan"] = 0;
      }
      else {
        data["heater"] = datafuzzy.fisOutputHeater;
        data["fan"] = datafuzzy.fisOutputFan;
      }
      data["callback_2"] = 1;
      data["callback_3"] = 1;
    }
    // Kirim data ke serial board atmega
    serializeJson(data, atmegaSerial);
  }
}

void getDataAtmega(void) {
  DynamicJsonDocument data(500);
  DeserializationError err = deserializeJson(data, atmegaSerial);
  if(!err) {
    callback_4 = data["callback_4"];
    if(callback_4 == 1){
      datafuzzy.powerHeater = data["dimheater"];
      datafuzzy.powerFan = data["dimfan"];
    }
  }
}