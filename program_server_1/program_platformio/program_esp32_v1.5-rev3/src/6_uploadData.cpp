/*! @file 6_uploadData.cpp
 * @version 1.5-rev3
*/

#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WebServer.h>
#include <WiFiClient.h>
#include <HTTPUpdateServer.h>
#include <ArduinoJson.h>
#include "1_configProgram.h"

// buat fungsi mengirim data
uint64_t waktuSebelum_uploadData = 0;
bool mDNSESPReady = false;

void send_data(String kodekubikel_input, 
float tegangan_input, float arus_input, float daya_input, float pF_input, float energy_input, float freq_input,
float temp_input, float hum_input, float dimmerHeater, float fisOutputHeater, float dimmerFan, float fisOutputFan)
{
    uint64_t waktuSekarang = millis();
    if (waktuSekarang - waktuSebelum_uploadData >= interval_uploadData) {
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
    send_data(kodekubikel, datapzem.V, datapzem.I, datapzem.P,
    datapzem.pF, datapzem.E, datapzem.Freq, datadht.temperature,
    datadht.humidity, datafuzzy.powerHeater, datafuzzy.fisOutputHeater,
    datafuzzy.powerFan, datafuzzy.fisOutputFan);
    server.send(200, "text/plain", page);
    page = "";
}

// buat fungsi page 404 jika page tidak ditemukan
void handleNotFound() {
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
void server_setup(void) {
    // Set up mDNS responder:
    // - first argument is the domain name, in this example
    //   the fully-qualified domain name is "esp32.local"
    // - second argument is the IP address to advertise
    //   we send our IP address on the WiFi network
    if (!MDNS.begin("esp32-webupdate")) {
        Serial.println(F("Error setting up MDNS responder!"));
        while (true) {
            delay(1000);
        }
    }
    mDNSESPReady = true;
    Serial.println(F("mDNS responder started"));

    // root page
    server.on("/", []() {
        server.sendHeader("Location", "http://" + String(ipAddress) + "/help");
        server.send(303);
    });

    server.on("/" + String(kodekubikel), handleRoot);
    // server.on("/", handleRoot);

    // reset pzem page
    server.on("/resetpzem", []() {
        resetpzem();
        page = "{\"pzem_state\": \""+String(1)+"\", \"reason\": \""+String("pzem energy has restarted....")+"\"}";
        server.send(200, "text/plain", page);
        delay(20);
        page = "";
    });

    // reset server
    server.on("/restarthardware", []() {
        page = "{\"restart_state\": \""+String(1)+"\", \"reason\": \""+String("Server ")+ String(kodekubikel) +String(" has restarted....")+"\"}";
        server.send(200, "text/plain", page);
        restartESP();
    });

    // disable program fuzzy
    server.on("/disablefuzzy", []() {
        page = "{\"fuzzy_state\": \""+String(0)+"\", \"reason\": \""+String("fuzzy logic has been disable....")+"\"}";
        server.send(200, "text/plain", page);
        stateFuzzy = false;
        saveConfig("fuzzy", false);
        page = "";
        NotifFuzzy(pin_buzzer, false);
    });

    // enable program fuzzy
    server.on("/enablefuzzy", []() {
       page = "{\"fuzzy_state\": \""+String(1)+"\", \"reason\": \""+String("fuzzy logic has been enable....")+"\"}";
        server.send(200, "text/plain", page);
        stateFuzzy = true;
        saveConfig("fuzzy", true);
        page = ""; 
        NotifFuzzy(pin_buzzer, true);
    });

    // disable buzzer speaker
    server.on("/disablebuzzer", []() {
        page = "{\"buzzer_state\": \""+String(0)+"\", \"reason\": \""+String("buzzer speaker has been disable....")+"\"}";
        server.send(200, "text/plain", page);
        buzzerSwitch = false;
        saveConfig("buzzer", false);
        page = "";
    });

    // enable buzzer speaker
    server.on("/enablebuzzer", []() {
        page = "{\"buzzer_state\": \""+String(1)+"\", \"reason\": \""+String("buzzer speaker has been enable....")+"\"}";
        server.send(200, "text/plain", page);
        buzzerSwitch = true;
        saveConfig("buzzer", true);
        page = ""; 
        NotifBuzzer(pin_buzzer);
    });

    // config WiFi server
    server.on("/config-wifi", HTTP_GET, []() {
        bool resetESP = false;
        String html = "<!DOCTYPE>\r\n<html>\r\n<head>\r\n<meta charset='utf-8'>\r\n<meta name='viewport' content='width=device-width, initial-scale=1.0'>\r\n";
        html += "<meta name='description' content='configWiFi'>\r\n<meta name='author' content='PHPGurukul'>\r\n";
        html += "<title>WiFi Configuration - TempCubiclePi "+String(version)+"</title>\r\n</head>\r\n";
        html += "<body>\r\n<h4>WiFi Configuration - TempCubiclePi "+String(version)+"</h4>\r\n";
        if(WiFi.getMode() != WIFI_AP) {
            html += "<p>Server kubikel " + String(kodekubikel) + " akan di restart dan ganti mode WIFI AP terlebih dahulu...<br /> silahkan koneksikan WiFi : " + String(STASSID) + " dan Password " + String(STAPSK) + "</p>\r\n";
            html += "<p>Setelah terkoneksi ke WiFi " +String(STASSID)+ ", silahkan klik <a href=http://192.168.4.1 target='_blank'> Helper Page </a></p>\r\n";
            resetESP = true;
            saveConfig("changeMode", true);
            
        } else {
            html += "<p><b>Old WiFi Configuration</b></p>\r\n";
            html += "<p><b> SSID : </b> " + String(loadSSID()) + "</p> \r\n";
            html += "<p><b> Password : </b> "+ String(loadPassword()) + "</p><br /> \r\n";
            html += "<form method='POST' action='/config-wifi-save'>\r\n";
            html += "<p><b>New Configuration : </b></p>\r\n";
            html += "<p>SSID: <input type='text' name='newssid'></p>\r\n";
            html += "<p>Password: <input type='password' name='newpassword'></p><br>\r\n";
            html += "<button type='submit' value='save'>Save</button>\r\n";
            html += "</form>\r\n";
        }
        html += "<br><p><b>Powered by : <a href=https://github.com/basyair7 target='_blank'>Basyair7</a></b></p>\r\n";
        html += "</body>\r\n</html>";
        server.send(200, "text/html", html);
        delay(1000);
        if(resetESP) restartESP();
    });

    // save new wifi configuration
    server.on("/config-wifi-save", HTTP_POST, []() {
        String new_ssid = server.arg("newssid");
        String new_password = server.arg("newpassword");

        // save to configWifi.json
        saveConfigWiFi(new_ssid, new_password);
        saveConfig("changeMode", false);

        String html = "<!DOCTYPE html>\r\n<html>\r\n<head>\r\n<meta charset='utf-8'>\r\n<meta name='viewport' content='width=device-width, initial-scale=1.0'>\r\n";
        html += "<meta name='description' content='configWiFi'>\r\n<meta name='author' content='PHPGurukul'>\r\n";
        html += "<title>WiFi Configuration - TempCubiclePi "+String(version)+"</title>\r\n</head>\r\n";
        html += "<body>\r\n<h4>WiFi Configuration - TempCubiclePi "+String(version)+"</h4>\r\n";
        html += "<h4> Data telah di simpan </h4>\r\n";
        html += "<p> WiFi SSID : " + String(new_ssid) + "</p>\r\n";
        html += "<p> Password : " + String(new_password) + "</p>\r\n";
        html += "<a href=http://"+String(ipAddress)+"/help>Kembali ke Menu</a>\r\n";
        html += "<br><p><b>Powered by : <a href=https://github.com/basyair7 target='_blank'>Basyair7</a></b></p>\r\n";
        html += "</body>\r\n</html>";

        server.send(200, "text/html", html);
    });

    // help configurate
    server.on("/help", []() {
        String helpPage = "<!DOCTYPE html>\r\n<html>\r\n<head>\r\n<meta charset='utf-8'>\r\n<meta name='viewport' content='width=device-width, initial-scale=1.0'>\r\n";
        helpPage += "<meta name='description' content='configWiFi'>\r\n<meta name='author' content='PHPGurukul'>\r\n";
        helpPage += "<title>Helper Page - TempCubiclePi "+String(version)+"</title>\r\n</head>\r\n";
        helpPage += "<body>\r\n<h4>Helper Page - TempCubiclePi "+String(version)+"<br /> TCP server started : </h4>";
        helpPage += "<p><a href=http://"+ String(ipAddress) +"/"+ String(kodekubikel) +"> IPAddress : " + String(ipAddress) + "/" + String(kodekubikel) + "</a></p>\r\n";
        helpPage += "<p>1. Reset PZEM on Server : <a href=http://"+ String(ipAddress) + "/resetpzem>http://"+ String(ipAddress) + "/resetpzem</a></p>\r\n";
        helpPage += "<p>2. Restart Hardware on Server : <a href=http://"+ String(ipAddress) + "/restarthardware>http://"+ String(ipAddress) + "/restarthardware</a></p>\r\n";
        helpPage += "<p>3. Disable Program Fuzzy : <a href=http://"+ String(ipAddress) + "/disablefuzzy>http://"+ String(ipAddress) + "/disablefuzzy</a></p>\r\n";
        helpPage += "<p>4. Enable Program Fuzzy : <a href=http://"+ String(ipAddress) + "/enablefuzzy>http://"+ String(ipAddress) + "/enablefuzzy</a></p>\r\n";
        helpPage += "<p>5. Disable Buzzer Speaker : <a href=http://"+ String(ipAddress) + "/disablebuzzer>http://"+ String(ipAddress) + "/disablebuzzer</a></p>\r\n";
        helpPage += "<p>6. Enable Buzzer Speaker : <a href=http://"+ String(ipAddress) + "/enablebuzzer>http://"+ String(ipAddress) + "/enablebuzzer</a></p>\r\n";
        helpPage += "<p>7. Enable Auto Change Mode WiFi : Coming Soon </p>";
        helpPage += "<p>8. Disable Auto Change Mode WiFi : Coming Soon </p>";
        helpPage += "<p>9. Configuration WiFi : <a href=http://"+ String(ipAddress) + "/config-wifi"; 
        helpPage += (WiFi.getMode() != WIFI_AP ? " onclick=\"return confirm('Server akan direstart dan beralih mode WiFi AP... tetap dilanjutkan?')\">" : ">");
        helpPage += "http://"+ String(ipAddress) + "/config-wifi</a></p>\r\n";
        helpPage += "<br><p><b>Powered by : <a href=https://github.com/basyair7 target='_blank'>Basyair7</a></b></p>\r\n";
        helpPage += "</body>\r\n</html>";

        server.send(200, "text/html", helpPage);
    });



    // Start TCP (HTTP) server
    httpUpdater.setup(&server);
    server.begin();
    server.onNotFound(handleNotFound);
    Serial.print(F("TCP server started : "));
    Serial.println(String(ipAddress)+"/"+String(kodekubikel)+"\n");
    Serial.print(F("Help Page : "));
    Serial.println(String(ipAddress)+"/help");

    // Add service to MDNS-SD
    MDNS.addService("http", "tcp", 80);
}