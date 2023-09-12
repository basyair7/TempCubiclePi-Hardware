/*! @file SerialData.ino
 * @version 1.5-rev2
*/

void getDataESP()
{
  // buat object DynamicJsonDocument data
  DynamicJsonDocument data(500);
  // deserializeJson/parse data json
  deserializeJson(data, Serial);
  // ambil data dari esp32
  getPower_ACFan = data["fan"];
  getPower_ACHeater = data["heater"];
  callback_2 = data["callback_2"];
  callback_3 = data["callback_3"];
}

void sendSerialData(long millisMain, int powerHeater, int powerFan)
{
  if ((unsigned long) (millisMain - waktuSebelum_2) >= interval_2) 
  {
    waktuSebelum_2 = millisMain;
    // Buat object DynamicJsonDocument data
    DynamicJsonDocument data(500);
    if(callback_3 == 1) {
      // Masukan nilai data ke dalam format json
      data["callback_4"] = 1;
      data["dimfan"] = powerFan;
      data["dimheater"] = powerHeater;
      // Kirim data ke serial komunikasi
      serializeJson(data, Serial);
    }
  }
}

// buat fungsi sendCallback ke ESP32
void sendCallback(void) {
  DynamicJsonDocument data(500);
  data["callback_1"] = 1;
  serializeJson(data, Serial);
}