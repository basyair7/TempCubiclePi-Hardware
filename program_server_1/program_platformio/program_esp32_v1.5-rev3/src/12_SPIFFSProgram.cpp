/*! @file 12_SPIFFSProgram.cpp
 * @version 1.5-rev3
 */

#include <Arduino.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>

/* Membaca nilai config
 * stateFuzzy = "fuzzy", statebuzzer = "buzzer", stateWifi = "wifi"
 * SelfChangeMode = "changeMode"
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
    if(!err) {
        if(stateConfig == "fuzzy") {
            bool value = doc["stateFuzzy"];
            return value;
        }
        if(stateConfig == "buzzer") {
            bool value = doc["stateBuzzer"];
            return value;
        }
        if(stateConfig == "wifi") {
            bool value = doc["stateWiFi"];
            return value;
        }
        if(stateConfig == "changeMode") {
            bool value = doc["stateSelfChangeMode"];
            return value;
        }
    }
    else {
        Serial.println(F("Gagal mengurai data JSON"));
        return true;
    }
}

/* Menyimpan nilai config
 * stateFuzzy = "fuzzy", statebuzzer = "buzzer", stateWifi = "wifi"
 * SelfChangeMode = "changeMode"
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
        bool stateSelfChangeModeValue = data["stateSelfChangeMode"];
        data["stateFuzzy"] = value;
        data["stateBuzzer"] = stateBuzzerValue;
        data["stateWiFi"] = stateWiFiValue;
        data["stateSelfChangeMode"] = stateSelfChangeModeValue;
    }

    if(stateConfig == "buzzer" && !err) {
        bool stateFuzzyValue = data["stateFuzzy"];
        bool stateWiFiValue = data["stateWiFi"];
        bool stateSelfChangeModeValue = data["stateSelfChangeMode"];
        data["stateFuzzy"] = stateFuzzyValue;
        data["stateBuzzer"] = value;
        data["stateWiFi"] = stateWiFiValue;
        data["stateSelfChangeMode"] = stateSelfChangeModeValue;
    }

    if(stateConfig == "wifi" && !err) {
        bool stateBuzzerValue = data["stateBuzzer"];
        bool stateFuzzyValue = data["stateFuzzy"];
        bool stateSelfChangeModeValue = data["stateSelfChangeMode"];
        data["stateFuzzy"] = stateFuzzyValue;
        data["stateBuzzer"] = stateBuzzerValue;
        data["stateWiFi"] = value;
        data["stateSelfChangeMode"] = stateSelfChangeModeValue;
    }

    if(stateConfig == "changeMode" && !err) {
        bool stateBuzzerValue = data["stateBuzzer"];
        bool stateFuzzyValue = data["stateFuzzy"];
        bool stateWiFiValue = data["stateWiFi"];
        data["stateFuzzy"] = stateFuzzyValue;
        data["stateBuzzer"] = stateBuzzerValue;
        data["stateWiFi"] = stateWiFiValue;
        data["stateSelfChangeMode"] = value;
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

void saveConfigWiFi(String ssid, String password) {
    DynamicJsonDocument config(500);
    config["ssid"] = String(ssid);
    config["password"] = String(password);
    File configFileWiFi = SPIFFS.open("/config/configwifi.json", "w");
    if (!configFileWiFi) {
        Serial.println(F("\nFailed to open config file for writing"));
        return;
    }
    serializeJson(config, configFileWiFi);
    configFileWiFi.close();
}

// load config WiFi
String loadConfigWiFi() {
    if(!SPIFFS.begin(true)) {
        Serial.println(F("\nFailed to mount SPIFFS"));
        return "";
    }
    if(!SPIFFS.exists("/config/configwifi.json")) {
        Serial.println(F("\nfile configwifi.json not found"));
        return "";
    }

    File wifiSSIDconfigFile = SPIFFS.open("/config/configwifi.json", "r");
    if(!wifiSSIDconfigFile) {
        Serial.println(F("\nFailed to open config file"));
        return "";
    }

    size_t size = wifiSSIDconfigFile.size();
    std::unique_ptr<char[]> buf(new char[size]);
    wifiSSIDconfigFile.readBytes(buf.get(), size);
    
    DynamicJsonDocument data(500);
    DeserializationError err = deserializeJson(data, buf.get());
    wifiSSIDconfigFile.close();
    
    if(!err) {
        return data["ssid"], data["password"];
    }
    else {
        Serial.println(F("\nFailed to load SSID and Password"));
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
    DynamicJsonDocument data(200);
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
    DynamicJsonDocument data(200);
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
        data["stateSelfChangeMode"] = false;
        
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

/*
bool loadConfigWiFi() {
    DynamicJsonDocument data(200);
    File configFile = SPIFFS.open("/configWifi.json", "r");
    if(!configFile) {
        Serial.println(F("Failed to open config file"));
        return false;
    }

    size_t size = configFile.size();
    std::unique_ptr<char[]> buffer(new char[size]);

    configFile.readBytes(buffer.get(), size);
    configFile.close();

    DeserializationError error = deserializeJson(data, buffer.get());
    if(!error) {
        char* newSSID = data["ssid"];
        char* newPassword = data["password"];

        return true;
    }
    else {
        Serial.println(F("Failed to read config file"));
        return false;
    }
}
*/
