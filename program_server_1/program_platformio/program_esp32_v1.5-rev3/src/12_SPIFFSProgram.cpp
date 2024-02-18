/*! @file 12_SPIFFSProgram.cpp
 * @version 1.5-rev3
 */

#include <Arduino.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>

/* Membaca nilai config
 * stateFuzzy = "fuzzy", statebuzzer = "buzzer", stateWifi = "wifi"
 * ChangeModeWiFi = "changeMode"
*/
bool readConfig(String stateConfig) {
    File configFile = SPIFFS.open("/config/config.json", "r");
    if (!configFile) {
        Serial.println("Berkas konfigurasi tidak ada");
        return true;
    }
    size_t size = configFile.size();
    std::unique_ptr<char[]> buf(new char[size]);

    configFile.readBytes(buf.get(), size);
    configFile.close();

    DynamicJsonDocument doc(200);
    DeserializationError err = deserializeJson(doc, buf.get());
    if(!err && stateConfig == "fuzzy") {
        bool value = doc["stateFuzzy"];
        return value;
    }
    if(!err && stateConfig == "buzzer") {
        bool value = doc["stateBuzzer"];
        return value;
    }
    if(!err && stateConfig == "wifi") {
        bool value = doc["stateWiFi"];
        return value;
    }
    if(!err && stateConfig == "changeMode") {
        bool value = doc["stateChangeMode"];
        return value;
    }
    if(!err && stateConfig == "autoChangeMode") {
        bool value = doc["stateAutoChangeMode"];
        return value;
    }
    else {
        Serial.println(F("Gagal mengurai data JSON"));
        return true;
    }
}

/* Menyimpan nilai config
 * stateFuzzy = "fuzzy", statebuzzer = "buzzer", stateWifi = "wifi"
 * ChangeModeWiFi = "changeMode", autoChangeModeWifi = autoChangeMode
*/
void saveConfig(String stateConfig, bool value) {
    File oldconfigFile = SPIFFS.open("/config/config.json", "r");
    if (!oldconfigFile) {
        Serial.println("\nBerkas konfigurasi tidak ada");
        return;
    }
    size_t size = oldconfigFile.size();
    std::unique_ptr<char[]> buf(new char[size]);

    oldconfigFile.readBytes(buf.get(), size);
    oldconfigFile.close();

    DynamicJsonDocument data(200);
    DeserializationError err = deserializeJson(data, buf.get());

    if(stateConfig == "fuzzy" && !err) {
        bool stateBuzzerValue = data["stateBuzzer"];
        bool stateWiFiValue = data["stateWiFi"];
        bool stateChangeModeValue = data["stateChangeMode"];
        bool stateAutoChangeModeValue = data["stateAutoChangeMode"];
        data["stateFuzzy"] = value;
        data["stateBuzzer"] = stateBuzzerValue;
        data["stateWiFi"] = stateWiFiValue;
        data["stateChangeMode"] = stateChangeModeValue;
        data["stateAutoChangeMode"] = stateAutoChangeModeValue;
    }

    if(stateConfig == "buzzer" && !err) {
        bool stateFuzzyValue = data["stateFuzzy"];
        bool stateWiFiValue = data["stateWiFi"];
        bool stateChangeModeValue = data["stateChangeMode"];
        bool stateAutoChangeModeValue = data["stateAutoChangeMode"];
        data["stateFuzzy"] = stateFuzzyValue;
        data["stateBuzzer"] = value;
        data["stateWiFi"] = stateWiFiValue;
        data["stateChangeMode"] = stateChangeModeValue;
        data["stateAutoChangeMode"] = stateAutoChangeModeValue;
    }

    if(stateConfig == "wifi" && !err) {
        bool stateBuzzerValue = data["stateBuzzer"];
        bool stateFuzzyValue = data["stateFuzzy"];
        bool stateChangeModeValue = data["stateChangeMode"];
        bool stateAutoChangeModeValue = data["stateAutoChangeMode"];
        data["stateFuzzy"] = stateFuzzyValue;
        data["stateBuzzer"] = stateBuzzerValue;
        data["stateWiFi"] = value;
        data["stateChangeMode"] = stateChangeModeValue;
        data["stateAutoChangeMode"] = stateAutoChangeModeValue;
    }

    if(stateConfig == "changeMode" && !err) {
        bool stateBuzzerValue = data["stateBuzzer"];
        bool stateFuzzyValue = data["stateFuzzy"];
        bool stateWiFiValue = data["stateWiFi"];
        bool stateAutoChangeModeValue = data["stateAutoChangeMode"];
        data["stateFuzzy"] = stateFuzzyValue;
        data["stateBuzzer"] = stateBuzzerValue;
        data["stateWiFi"] = stateWiFiValue;
        data["stateChangeMode"] = value;
        data["stateAutoChangeMode"] = stateAutoChangeModeValue;
    }

    if(stateConfig == "autoChangeMode" && !err) {
        bool stateBuzzerValue = data["stateBuzzer"];
        bool stateFuzzyValue = data["stateFuzzy"];
        bool stateWiFiValue = data["stateWiFi"];
        bool stateChangeModeValue = data["stateChangeMode"];
        data["stateFuzzy"] = stateFuzzyValue;
        data["stateBuzzer"] = stateBuzzerValue;
        data["stateWiFi"] = stateWiFiValue;
        data["stateChangeMode"] = stateChangeModeValue;
        data["stateAutoChangeMode"] = value;
    }

    size_t serializeSize = measureJson(data);
    if(serializeSize > 1048) {
        Serial.println(F("\nUkuran data konfigurasi melebihi dari batas (1kb)"));
        return;
    }

    File configFileUpdate = SPIFFS.open("/config/config.json", "w");
    if(!configFileUpdate) {
        Serial.println("\nBerkas konfigurasi tidak ada");
        return;
    }

    serializeJson(data, configFileUpdate);
    configFileUpdate.close();
}

// Save Client WiFi
void saveConfigWiFi(String ssid, String password) {
    DynamicJsonDocument config(100);
    config["ssid"] = String(ssid);
    config["password"] = String(password);
    File configFileWiFi = SPIFFS.open("/config/configwifi.json", "w");
    if (!configFileWiFi) {
        Serial.println(F("\nFailed to open config WiFi for writing"));
        return;
    }
    serializeJson(config, configFileWiFi);
    configFileWiFi.close();
}

// Save Access Point WiFi
void saveConfigAP(String ssid, String password) {
    DynamicJsonDocument config(100);
    config["ssid"] = String(ssid);
    config["password"] = String(password);
    File configFileWiFi = SPIFFS.open("/config/configAP.json", "w");
    if (!configFileWiFi) {
        Serial.println(F("\nFailed to open config Access Point for writing"));
        return;
    }
    serializeJson(config, configFileWiFi);
    configFileWiFi.close();
}

String loadAPName() {
    File wifiPSKconfigFile = SPIFFS.open("/config/configAP.json", "r");
    if(!wifiPSKconfigFile) {
        Serial.println(F("\nFailed to open config file AP Name"));
        return "";
    }
    size_t size = wifiPSKconfigFile.size();
    std::unique_ptr<char[]> buf(new char[size]);
    wifiPSKconfigFile.readBytes(buf.get(), size);
    wifiPSKconfigFile.close();
    DynamicJsonDocument data(50);
    DeserializationError err = deserializeJson(data, buf.get());
    if(!err) {
        String value = data["ssid"];
        return value;
    }
    else {
        Serial.println(F("\nFailed to load AP Name"));
        return "";
    }
}

String loadAPPassword() {
    File wifiPSKconfigFile = SPIFFS.open("/config/configAP.json", "r");
    if(!wifiPSKconfigFile) {
        Serial.println(F("\nFailed to open config file AP Password"));
        return "";
    }
    size_t size = wifiPSKconfigFile.size();
    std::unique_ptr<char[]> buf(new char[size]);
    wifiPSKconfigFile.readBytes(buf.get(), size);
    wifiPSKconfigFile.close();
    DynamicJsonDocument data(50);
    DeserializationError err = deserializeJson(data, buf.get());
    if(!err) {
        String value = data["password"];
        return value;
    }
    else {
        Serial.println(F("\nFailed to load AP Password"));
        return "";
    }
}

// load config Client WiFi
String loadSSID() {
    File wifiPSKconfigFile = SPIFFS.open("/config/configwifi.json", "r");
    if(!wifiPSKconfigFile) {
        Serial.println(F("\nFailed to open config file"));
        return "";
    }
    size_t size = wifiPSKconfigFile.size();
    std::unique_ptr<char[]> buf(new char[size]);
    wifiPSKconfigFile.readBytes(buf.get(), size);
    wifiPSKconfigFile.close();
    DynamicJsonDocument data(50);
    DeserializationError err = deserializeJson(data, buf.get());
    if(!err) {
        String value = data["ssid"];
        return value;
    }
    else {
        Serial.println(F("\nFailed to load SSID"));
        return "";
    }
}

String loadPassword() {
    File wifiPSKconfigFile = SPIFFS.open("/config/configwifi.json", "r");
    if(!wifiPSKconfigFile) {
        Serial.println(F("\nFailed to open config file"));
        return "";
    }
    size_t size = wifiPSKconfigFile.size();
    std::unique_ptr<char[]> buf(new char[size]);
    wifiPSKconfigFile.readBytes(buf.get(), size);
    wifiPSKconfigFile.close();
    DynamicJsonDocument data(50);
    DeserializationError err = deserializeJson(data, buf.get());
    if(!err) {
        String value = data["password"];
        return value;
    }
    else {
        Serial.println(F("\nFailed to load Password"));
        return "";
    }
}

// save kubikel code
void saveKubikelCode(String code) {
    DynamicJsonDocument config(50);
    config["kubikelcode"] = String(code);
    File configFile = SPIFFS.open("/config/kubikelcode.json", "w");
    if (!configFile) {
        Serial.println(F("\nFailed to open config file for writing"));
        return;
    }
    serializeJson(config, configFile);
    configFile.close();
}

// load kubikel code
String loadKubikelCode() {
    File configFile = SPIFFS.open("/config/kubikelcode.json", "r");
    if(!configFile) {
        Serial.println(F("\nFailed to open config file load kubikel"));
        return "";
    }
    size_t size = configFile.size();
    std::unique_ptr<char[]> buf(new char[size]);
    configFile.readBytes(buf.get(), size);
    configFile.close();
    DynamicJsonDocument data(50);
    DeserializationError err = deserializeJson(data, buf.get());
    if(!err) {
        String value = data["kubikelcode"];
        return value;
    }
    else {
        Serial.println(F("\nFailed to load kubikel code"));
        return "";
    }
}

// remove file in SPIFFS
void removeSPIFFS(String path) {
    if(SPIFFS.remove(path)){
        Serial.println("\nData " + String(path) + " telah dihapus");
    } else Serial.println(F("\nData tidak tersedia"));
}

// read ALL file SPIFFS
void listSPIFFSFiles() {
    if (!SPIFFS.begin(true)) {
        Serial.println(F("\nFailed to mount SPIFFS"));
        return;
    }
    Serial.println(F("\nList Directiory File SPIFFS : "));
    File root = SPIFFS.open("/");
    File file = root.openNextFile();
    while(file) {
        Serial.print(F("File : "));
        Serial.println(String(file.name()) +"("+ String(file.size()) + ")");
        file = root.openNextFile();
    }
}

// open file SPIFFS
void openFileSPIFFS(const char* fileName) {
    if(!SPIFFS.begin(true)) {
        Serial.println(F("Failed to mount SPIFFS"));
        return;
    }

    File file = SPIFFS.open(fileName, "r");
    if(!file) {
        Serial.println("Failed open File " + String(fileName));
        return;
    }
    Serial.print(F("File Value : "));
    while(file.available()) {
        Serial.write(file.read());
    }
    Serial.println();
    file.close();
}

void setupSPIFFS(void)
{
    if (!SPIFFS.begin(true))
    {
        Serial.println(F("Failed to mount file system"));
        return;
    }

    if(!SPIFFS.exists("/config/config.json")) {
        DynamicJsonDocument data(200);
        data["stateFuzzy"] = true;
        data["stateBuzzer"] = true;
        data["stateWiFi"] = false;
        data["stateChangeMode"] = false;
        
        File configFile = SPIFFS.open("/config/config.json", "w");
        if(!configFile) {
            Serial.println(F("Gagal membuat berkas config.json"));
            return;
        }
        serializeJson(data, configFile);
        Serial.println(F("Berkas config.json telah disimpan"));
    }
    listSPIFFSFiles();
}
