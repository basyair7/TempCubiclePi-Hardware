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
#include "WebPage.h"

// buat fungsi mengirim data
unsigned long waktuSebelum_uploadData = 0;
bool mDNSESPReady = false;

void send_data(String kodekubikel_input, 
float tegangan_input, float arus_input, float daya_input, float pF_input, float energy_input, float freq_input,
float temp_input, float hum_input, float dimmerHeater, float fisOutputHeater, float dimmerFan, float fisOutputFan)
{
    unsigned long waktuSekarang = millis();
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

    //1. reset pzem page
    server.on("/resetpzem", []() {
        resetpzem();
        page = "{\"pzem_state\": \""+String(1)+"\", \"reason\": \""+String("pzem energy has restarted....")+"\"}";
        server.send(200, "text/plain", page);
        delay(20);
        page = "";
    });

    //2. reset server
    server.on("/restarthardware", []() {
        page = "{\"restart_state\": \""+String(1)+"\", \"reason\": \""+String("Server ")+ String(kodekubikel) +String(" has restarted....")+"\"}";
        server.send(200, "text/plain", page);
        restartESP();
    });

    //3. disable program fuzzy
    server.on("/disablefuzzy", []() {
        page = "{\"fuzzy_state\": \""+String(0)+"\", \"reason\": \""+String("fuzzy logic has been disable....")+"\"}";
        server.send(200, "text/plain", page);
        stateFuzzy = false;
        saveConfig("fuzzy", false);
        page = "";
        NotifFuzzy(pin_buzzer, false);
    });

    //4. enable program fuzzy
    server.on("/enablefuzzy", []() {
       page = "{\"fuzzy_state\": \""+String(1)+"\", \"reason\": \""+String("fuzzy logic has been enable....")+"\"}";
        server.send(200, "text/plain", page);
        stateFuzzy = true;
        saveConfig("fuzzy", true);
        page = ""; 
        NotifFuzzy(pin_buzzer, true);
    });

    //5. disable buzzer speaker
    server.on("/disablebuzzer", []() {
        page = "{\"buzzer_state\": \""+String(0)+"\", \"reason\": \""+String("buzzer speaker has been disable....")+"\"}";
        server.send(200, "text/plain", page);
        buzzerSwitch = false;
        saveConfig("buzzer", false);
        page = "";
    });

    //6. enable buzzer speaker
    server.on("/enablebuzzer", []() {
        page = "{\"buzzer_state\": \""+String(1)+"\", \"reason\": \""+String("buzzer speaker has been enable....")+"\"}";
        server.send(200, "text/plain", page);
        buzzerSwitch = true;
        saveConfig("buzzer", true);
        page = ""; 
        NotifBuzzer(pin_buzzer);
    });

    //7. enable Auto Change Mode WiFi
    server.on("/enableautochangemodewifi", []() {
        page = "{\"auto_changeMode\": \""+String(1)+"\", \"reason\": \""+String("Auto Change Mode WiFi AP has been enable....")+"\"}";
        server.send(200, "text/plain", page);
        saveConfig("autoChangeMode", true);
        digitalWrite(LED_BUILTIN, LOW);
        delay(1000);
        digitalWrite(LED_BUILTIN, HIGH);
        page = "";
    });

    //8. disable Auto Change Mode WiFi
    server.on("/disableautochangemodewifi", []() {
        page = "{\"auto_changeMode\": \""+String(0)+"\", \"reason\": \""+String("Auto Change Mode WiFi AP has been disable....")+"\"}";
        server.send(200, "text/plain", page);
        saveConfig("autoChangeMode", false);
        digitalWrite(LED_BUILTIN, LOW);
        delay(1000);
        digitalWrite(LED_BUILTIN, HIGH);
        page = "";
    });

    //9. config WiFi server
    server.on("/config-wifi", HTTP_GET, []() {

        bool resetESP = false;
        if(WiFi.getMode() != WIFI_AP) {
            String a_tags[6] = {
                version, version,
                kodekubikel, STASSID, STAPSK,
                STASSID
            };

            // menghitung ukuran buffer
            int buffer_size = sizeof(config_wifi_1);
            for(uint8_t i = 0; i < 5; i++){
                buffer_size += a_tags[i].length();
            }
            
            // membuat typedata config_wifi_page untuk menyimpan hasil sprintf
            char config_wifi_page[buffer_size];

            // menggunakan sprintf untuk menggabungkan ke char config_wifi_1
            sprintf(
                config_wifi_page, config_wifi_1,
                a_tags[0].c_str(), a_tags[1].c_str(), a_tags[2].c_str(),
                a_tags[3].c_str(), a_tags[4].c_str(), a_tags[5].c_str()
            );

            resetESP = true;
            saveConfig("changeMode", true);

            server.send(200, "text/html", config_wifi_page);

        } else {
            String a_tags[5] = {
                version, version,
                ssid, password,
                ipAddress
            };

            // menghitung ukuran buffer
            int buffer_size = sizeof(config_wifi_2);
            for(uint8_t i = 0; i < 4; i++){
                buffer_size += a_tags[i].length();
            }

            // membuat typedata config_wifi_page untuk menyimpan hasil sprintf
            char config_wifi_page[buffer_size];
            
            // menggunakan sprintf untuk menggabungkan ke char config_wifi_2
            sprintf(
                config_wifi_page, config_wifi_2,
                a_tags[0].c_str(), a_tags[1].c_str(), a_tags[2].c_str(),
                a_tags[3].c_str(), a_tags[4].c_str()
            );

            server.send(200, "text/html", config_wifi_page);
        }

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

        String a_tags[4] = {
            version,
            new_ssid, new_password,
            ipAddress
        };

        // menghitung ukuran buffer
        int buffer_size = sizeof(success_save_config_wifi);
        for(uint8_t i = 0; i < 3; i++){
            buffer_size += a_tags[i].length();
        }

        // membuat typedata config_wifi_page untuk menyimpan hasil sprintf
        char config_wifi_page[buffer_size];

        // menggunakan sprintf untuk menggabungkan element a_tags ke success_save_config_wifi
        sprintf(
            config_wifi_page, success_save_config_wifi,
            a_tags[0].c_str(), a_tags[1].c_str(), a_tags[2].c_str(),
            a_tags[3].c_str()
        );

        server.send(200, "text/html", config_wifi_page);
    });

    // 10. change mode
    server.on("/apmode", []() {
        saveConfig("changeMode", true);
        page = "{\"changeMode\": \""+String(1)+"\", \"reason\": \""+String("Mode AP Success..")+"\"}";
        server.send(200, "text/javascript", page);
        digitalWrite(LED_BUILTIN, LOW);
        delay(1000);
        digitalWrite(LED_BUILTIN, HIGH);
        page = "";
    });

    // 10. change mode
    server.on("/clientmode", []() {
        saveConfig("changeMode", false);
        page = "{\"changeMode\": \""+String(0)+"\", \"reason\": \""+String("Mode Client Success..")+"\"}";
        server.send(200, "text/javascript", page);
        digitalWrite(LED_BUILTIN, LOW);
        delay(1000);
        digitalWrite(LED_BUILTIN, HIGH);
        page = "";
    });

    // 11. change kubikel code
    server.on("/config-ap", [](){
        String a_tags[5] = {
            version, version,
            APName, APPassword,
            ipAddress
        };

        // menghitung ukuran buffer
        int buffer_size = sizeof(config_ap);
        for(uint8_t i = 0; i < 4; i++) {
            buffer_size += a_tags[i].length();
        }

        // membuat type data config_ap_page untuk menyimpan hasil sprintf
        char config_ap_page[buffer_size];

        // menggunakan sprintf untuk menggabungkan element a_tags ke config_ap
        sprintf(
            config_ap_page, config_ap,
            a_tags[0].c_str(), a_tags[1].c_str(), a_tags[2].c_str(),
            a_tags[3].c_str(),a_tags[4].c_str()
        );

        // kirim ke server
        server.send(200, "text/html", config_ap_page);
    });

    server.on("/config-ap-save", HTTP_POST, [](){
        String new_ap = server.arg("newap");
        String new_password = server.arg("newpassword");
        
        // save to configap.json
        saveConfigAP(new_ap, new_password);

        String a_tags[4] = {
            version,
            new_ap, new_password,
            ipAddress
        };

        // menghitung ukuran buffer
        int buffer_size = sizeof(config_ap);
        for(uint8_t i = 0; i < 3; i++) {
            buffer_size += a_tags[i].length();
        }

        // membuat type data config_ap_page untuk menyimpan hasil sprintf
        char config_ap_page[buffer_size];

        // menggunakan sprintf untuk menggabungkan element a_tags ke config_ap
        sprintf(
            config_ap_page, success_save_config_ap,
            a_tags[0].c_str(), a_tags[1].c_str(), a_tags[2].c_str(),
            a_tags[3].c_str()
        );

        // kirim ke server
        server.send(200, "text/html", config_ap_page);
    });

    // 12. rename kubikel code
    server.on("/rename-kodekubikel", []() {
        String a_tags[4] = {
            version, version,
            kodekubikel,
            ipAddress
        };

        // menghitung ukuran buffer
        int buffer_size = sizeof(rename_kodekubikel);
        for(uint8_t i = 0; i < 3; i++) {
            buffer_size += a_tags[i].length();
        }

        // membuat type data rename_kodekubikel_page untuk menyimpan hasil sprintf
        char rename_kodekubikel_page[buffer_size];

        // menggunakan sprintf untuk menggabungkan element a_tags ke rename_kodekubikel_page
        sprintf(
            rename_kodekubikel_page, rename_kodekubikel,
            a_tags[0], a_tags[1], a_tags[2], a_tags[3]
        );

        // kirim ke server
        server.send(200, "text/html", rename_kodekubikel_page);
    });

    server.on("/save-rename-kodekubikel", HTTP_POST, []() {
        String new_code = server.arg("newcode");
        saveKubikelCode(new_code);
        String a_tags[3] = { version, new_code, ipAddress };

        // menghitung ukuran buffer
        int buffer_size = sizeof(success_save_rename_kodekubikel);
        for(byte i = 0; i < 2; i++) {
            buffer_size += a_tags[i].length();
        }

        // membuat type data success_save_page untuk menyimpan hasil sprintf
        char success_save_page[buffer_size];

        // meggunakan sprintf untuk menggabungkan element a_tags ke success_save_page
        sprintf(
            success_save_page, 
            success_save_rename_kodekubikel, 
            a_tags[0], a_tags[1], a_tags[2]
        );

        // kirim ke server
        server.send(200, "text/html", success_save_page);
        
    });

    // help configurate
    server.on("/help", []() {
        // masukkan element tag html ke dalam string c++
        String a_tags[26] = {
            version, version,
            FIRMWAREVERSION, BUILDTIME, FIRMWAREREGION, kodekubikel,
            String((SelfChangeMode == true ? "Enable" : "Disable")),
            String((stateFuzzy == true ? "Enable" : "Disable")),
            String((buzzerSwitch == true ? "Enable" : "Disable")),
            ipAddress, String((wifi_AP_mode == true ? "Access Point" : "Client WiFi")), APName, APPassword, ssid, password,
            "<p> TCP server started : <a href=http://"+ String(ipAddress) +"/" + String(kodekubikel) +" target='_blank'> http://" + String(ipAddress) + "/" + String(kodekubikel) + "</a></p>",
            "<p>1. Program Fuzzy : <a href=http://" + String(ipAddress) + "/enablefuzzy target='_blank'>Enable</a> / <a href=http://" + String(ipAddress) + "/disablefuzzy target='_blank'>Disable</a></p>",
            "<p>2. Buzzer Speaker : <a href=http://" + String(ipAddress) + "/enablebuzzer target='_blank'>Enable</a> / <a href=http://" + String(ipAddress) + "/disablebuzzer target='_blank'>Disable</a></p>",
            "<p>3. Mode WiFi : <a href=http://" + String(ipAddress) + "/apmode target='_blank'>Access Point</a> / <a href=http://" + String(ipAddress) + "/clientmode target='_blank'>Client WiFi</a></p>",
            "<p>4. Auto Change Mode WiFi : <a href=http://"+ String(ipAddress) + "/enableautochangemodewifi target='_blank'>Enable</a> / <a href=http://"+ String(ipAddress) + "/disableautochangemodewifi target='_blank'>Disable</a></p>",
            "<p>5. Rename Kubikel Code : <a href=http://" + String(ipAddress) + "/rename-kodekubikel >Run</a></p>",
            "<p>6. Configuration WiFi : <a href=http://" + String(ipAddress) + "/config-wifi"+
                String((WiFi.getMode() != WIFI_AP ? " onclick=\"return confirm('Server akan direstart dan beralih mode WiFi AP... tetap dilanjutkan?')\" >" : " >"))+
            "Run</a></p>",
            "<p>7. Configuration Access Point : <a href=http://" + String(ipAddress) + "/config-ap >Run</a>",
            "<p>8. Update Firmware ESP : <a href=http://" + String(ipAddress) + "/update target='_blank'>Update</a></p>"
            "<p>9. Restart Hardware on Server : <a href=http://"+ String(ipAddress) + "/restarthardware target='_blank'>Run</a></p>",
            "<p>10. Reset PZEM on Server : <a href=http://" + String(ipAddress) + "/resetpzem target='_blank'>Run</a></p>"
        };

        // menghitung ukuran buffer
        int buffer_size = sizeof(index_html);
        for(uint8_t i = 0; i < 25; i++) {
            buffer_size += a_tags[i].length();
        }

        // membuat typedata helpPage untuk menyimpan hasil sprintf
        char helpPage[buffer_size];

        // menggunakan sprintf untuk menggabungkan ke char index_html
        sprintf(
            helpPage, index_html,
            a_tags[0].c_str(), a_tags[1].c_str(), a_tags[2].c_str(), a_tags[3].c_str(), a_tags[4].c_str(),
            a_tags[5].c_str(), a_tags[6].c_str(), a_tags[7].c_str(), a_tags[8].c_str(), a_tags[9].c_str(),
            a_tags[10].c_str(), a_tags[11].c_str(), a_tags[12].c_str(), a_tags[13].c_str(), a_tags[14].c_str(),
            a_tags[15].c_str(), a_tags[16].c_str(), a_tags[17].c_str(), a_tags[18].c_str(), a_tags[19].c_str(),
            a_tags[20].c_str(), a_tags[21].c_str(), a_tags[22].c_str(), a_tags[23].c_str(), a_tags[24].c_str(),
            a_tags[25].c_str()
        );

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