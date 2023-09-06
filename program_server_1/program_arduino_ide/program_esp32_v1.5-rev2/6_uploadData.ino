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
    page = "{\"pzem_state\": \""+String(0)+"\", \"reason\": \""+String("pzem energy has restarted....")+"\"}";
    server.send(200, "text/plain", page);
    delay(20);
    page = "";    
  });

  // reset server
  server.on("/resetesp", []() {
    page = "{\"esp_state\": \""+String(0)+"\", \"reason\": \""+String("esp32 board has restarted....")+"\"}";
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

  // Start TCP (HTTP) server
  server.begin();
  server.onNotFound(handleNotFound);
  Serial.print(F("TCP server started : "));
  Serial.println(String(ipAddress)+"/"+String(kodekubikel));
  Serial.print(F("Reset PZEM on Server : "));
  Serial.println(String(ipAddress)+"/resetpzem");
  Serial.print(F("Reset Hardware on Server : "));
  Serial.println(String(ipAddress)+"/resetesp"); 
  Serial.print(F("Disable Program Fuzzy : "));
  Serial.println(String(ipAddress)+"/disablefuzzy");
  Serial.print(F("Enable Program Fuzzy : "));
  Serial.println(String(ipAddress)+"/enablefuzzy");

  // Add service to MDNS-SD
  MDNS.addService("http", "tcp", 80);
  
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

// buat fungsi mengirim data
void send_data(String kodekubikel_input, 
float tegangan_input, float arus_input, float daya_input, float pF_input, float energy_input, float freq_input,
float temp_input, float hum_input, float dimmerHeater, float fisOutputHeater, float dimmerFan, float fisOutputFan)
{
  unsigned long waktuSekarang = millis();
  if ((unsigned long) (waktuSekarang - waktuSebelum_uploadData) >= interval_uploadData) {
    waktuSebelum_uploadData = waktuSekarang;
    // buat object DynamicJsonDocument data
    DynamicJsonDocument data_page(500);

    // masukan data ke format json
    data_page["kubikel"] = kodekubikel_input;
    data_page["FISstate"] = (stateFuzzy == true ? "enable" : "disable");
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