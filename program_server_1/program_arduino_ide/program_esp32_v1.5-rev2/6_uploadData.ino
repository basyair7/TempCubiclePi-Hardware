/*! @file 6_uploadData.ino
 * @version 1.5-rev2
*/

// buat fungsi mengirim data
void send_data(String kodekubikel_input, float tegangan_input, float arus_input, float daya_input, float pF_input, 
float energy_input, float freq_input, float temp_input, float hum_input, float dimmerHeater, float fisOutputHeater,
float dimmerFan, float fisOutputFan) {
  unsigned long waktuSekarang = millis();
  if ((unsigned long) (waktuSekarang - waktuSebelum_uploadData) >= interval_uploadData) {
    waktuSebelum_uploadData = waktuSekarang;
    // buat object DynamicJsonDocument data
    DynamicJsonDocument data_page(500);

    // masukan data ke format json
    data_page["kubikel"] = kodekubikel_input;
    data_page["fuzzy"] = (stateFuzzy == true ? "enable" : "disable");
    data_page["buzzer"] = (buzzerSwitch == true ? "enable" : "disable");
    data_page["data_pzem"]["tegangan_heater"] = tegangan_input;
    data_page["data_pzem"]["arus_heater"] = arus_input;
    data_page["data_pzem"]["daya_heater"] = daya_input;
    data_page["data_pzem"]["energy_heater"] = energy_input;
    data_page["data_pzem"]["frequency_heater"] = freq_input;
    data_page["data_pzem"]["pf_heater"] = pF_input;
    data_page["dht"]["suhu"] = temp_input;
    data_page["dht"]["kelembapan"] = hum_input;
    data_page["atmega328p"]["dimmer_heater"] = dimmerHeater;
    data_page["atmega328p"]["fisOutputHeater"] = fisOutputHeater;
    data_page["atmega328p"]["dimmer_fan"] = dimmerFan;
    data_page["atmega328p"]["fisOutputFan"] = fisOutputFan;

    // masukan data ke variabel page
    serializeJson(data_page, page);
    delay(30);
  }
}

// kirim data ke server lokal
void handleRoot() {
  // masukan data ke dalam fungsi send_data
  send_data(
    kodekubikel, datapzem.V, datapzem.I, datapzem.P, datapzem.pF, datapzem.E, datapzem.Freq, datadht.temperature, datadht.humidity, 
    datafuzzy.powerHeater, datafuzzy.fisOutputHeater, datafuzzy.powerFan, datafuzzy.fisOutputFan
  );
  
  server.send(200, "text/plain", page);
  page = "";
}

// buat fungsi page 404 jika page tidak ditemukan
void handleNotFound(){
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

// konfigurasi web server
void server_setup(void){
  // Set up mDNS responder:
  // - first argument is the domain name, in this example
  //   the fully-qualified domain name is "esp32.local"
  // - second argument is the IP address to advertise
  //   we send our IP address on the WiFi network
  if (!MDNS.begin("esp32")) {
    Serial.println(F("Error setting up MDNS responder!"));
    while(1) {
      delay(1000);
    }
  }
  Serial.println(F("mDNS responder started"));

  // root page
  server.on("/" + String(kodekubikel), handleRoot);
  server.on("/", handleRoot);

  // reset pzem page
  server.on("/resetpzem", []() {
    resetpzem();
    page = "{\"pzem_state\": \""+String(1)+"\", \"reason\": \""+String("pzem energy has restarted....")+"\"}";
    server.send(200, "text/plain", page);
    delay(20);
    page = "";    
  });

  // reset server
  server.on("/resetesp", []() {
    page = "{\"esp_state\": \""+String(1)+"\", \"reason\": \""+String("esp32 board has restarted....")+"\"}";
    server.send(200, "text/plain", page);
    buzzer_shutdown(pin_buzzer);
    delay(1000);
    ESP.restart();
  });

  // disable program fuzzy
  server.on("/disablefuzzy", []() {
    page = "{\"fuzzy_state\": \""+String(0)+"\", \"reason\": \""+String("fuzzy logic has been disable....")+"\"}";
    server.send(200, "text/plain", page);
    stateFuzzy = false;
    EEPROM.write(0, 0);
    EEPROM.commit();
    page = "";
  });

  // enable program fuzzy
  server.on("/enablefuzzy", []() {
    page = "{\"fuzzy_state\": \""+String(1)+"\", \"reason\": \""+String("fuzzy logic has been enable....")+"\"}";
    server.send(200, "text/plain", page);
    stateFuzzy = true;
    EEPROM.write(0, 1);
    EEPROM.commit();
    page = "";
  });

  // disable buzzer speaker
  server.on("/disablebuzzer", []() {
    page ="{\"buzzer_state\": \""+String(0)+"\", \"reason\": \""+String("buzzer speaker has been disable...")+"\"}";
    server.send(200, "text/plain", page);
    buzzerSwitch = false;
    EEPROM.write(1, 0);
    EEPROM.commit();
    page = "";
  });
  
  // enable buzzer speaker
  server.on("/enablebuzzer", []() {
    page ="{\"buzzer_state\": \""+String(1)+"\", \"reason\": \""+String("buzzer speaker has been enable...")+"\"}";
    server.send(200, "text/plain", page);
    buzzerSwitch = true;
    EEPROM.write(1, 1);
    EEPROM.commit();
    page = "";
  });

  // help configurate
  server.on("/help", []() {
    String helpPage = "Helper Page\n\n";
    helpPage += "TCP server started : ";
    helpPage += String(ipAddress)+"/"+String(kodekubikel)+"\n";
    helpPage += "\n1. Reset PZEM on Server : ";
    helpPage += String(ipAddress)+"/resetpzem";
    helpPage += "\n2. Reset Hardware on Server : ";
    helpPage += String(ipAddress)+"/resetesp";
    helpPage += "\n3. Disable Program Fuzzy : ";
    helpPage += String(ipAddress)+"/disablefuzzy";
    helpPage += "\n4. Enable Program Fuzzy : ";
    helpPage += String(ipAddress)+"/enablefuzzy";
    helpPage += "\n5. Disable Buzzer Speaker : ";
    helpPage += String(ipAddress)+"/disablebuzzer";
    helpPage += "\n6. Enable Buzzer Speaker : ";
    helpPage += String(ipAddress)+"/enablebuzzer";

    server.send(200, "text/plain", helpPage);
  });

  // Start TCP (HTTP) server
  server.begin();
  server.onNotFound(handleNotFound);
  Serial.println("\n");
  Serial.print(F("TCP server started : "));
  Serial.println(String(ipAddress)+"/"+String(kodekubikel));
  Serial.print(F("Help page : "));
  Serial.println(String(ipAddress)+"/help");

  // Add service to MDNS-SD
  MDNS.addService("http", "tcp", 80);
  
}