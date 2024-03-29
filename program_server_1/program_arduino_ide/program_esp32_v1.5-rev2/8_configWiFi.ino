// program reconnect wifi server
void initWiFi() 
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  ipAddress = WiFi.localIP().toString().c_str();
}

// New Method : Reconnecting WiFi AP Server
void WiFiStationConnected(WiFiEvent_t event, WiFiEventInfo_t info) 
{
  Serial.println(F("Connected to AP Successfully!"));
  selfReset = 0;
}

void WiFiGotIP(WiFiEvent_t event, WiFiEventInfo_t info) 
{
  ipAddress = WiFi.localIP().toString().c_str();
  Serial.println(F("WiFi Connected"));
  Serial.print(F("IP Address : "));
  Serial.println(ipAddress);
}

void WiFiStationDisconnected(WiFiEvent_t event, WiFiEventInfo_t info)
{
  unsigned long current_time = millis();
  if ((WiFi.status() != WL_CONNECTED) && (current_time - previous_time >= interval_reconnect)) {
    Serial.println(F("Disconnected from WiFi Access Point"));
    Serial.print(F("WiFi lost connection, Reason"));
    Serial.println(info.wifi_sta_disconnected.reason);
    Serial.println(F("Trying to Reconnect"));
    WiFi.begin(ssid, password);   
    tone(pin_buzzer, 500, 100);
    tone(pin_buzzer, 2000, 20);
    noTone(pin_buzzer);
    if(selfReset > 5) ESP.restart();
    selfReset++;
    previous_time = current_time;
  }
}
/*! @file 8_configWiFi.ino
 * @version 1.5-rev2
*/

// Old Method : Reconnecting WiFi AP Server
void reconnectWiFi() 
{
  unsigned long current_time = millis(); // number of milliseconds since the upload
  // checking for WIFI connection
  if ((WiFi.status() != WL_CONNECTED) && (current_time - previous_time >= interval_reconnect)) {
    Serial.print(millis());
    Serial.println("Reconnecting to WIFI network");
    WiFi.disconnect();
    WiFi.reconnect();
    tone(pin_buzzer, 500, 100);    
    tone(pin_buzzer, 2000, 20);
    noTone(pin_buzzer);
    previous_time = current_time;
  }
}