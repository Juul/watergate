
#include <WiFi.h>
#include <HTTPClient.h>

#include "settings.h"

const char WIFI_SSID[] = SETTINGS_WIFI_SSID; // change in settings.h
const char WIFI_PASSWORD[] = SETTING_WIFI_PASSWORD; // change in settings.h

int watergate_request(int open) {
  HTTPClient http;

  if(open) {
    http.begin("HTTP://" + SETTINGS_WATERGATE_HOSTNAME + "/sf/on/w");
  } else { // send close request instead
    http.begin("HTTP://" + SETTINGS_WATERGATE_HOSTNAME + "/sf/off");
  }
  
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  int httpCode = http.GET();

  // httpCode will be negative on error
  if(httpCode > 0) {
    // file found at server
    if(httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      Serial.println(payload);
    } else {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTP] GET... code: %d\n", httpCode);
      http.end();
      return 1;
    }
  } else {
    Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    http.end();
    return 1;
  }

  http.end();
  
  return 0;
}


void setup() {
  //  pinMode(PIN_SF, OUTPUT);
  //  digitalWrite(PIN_SF, LOW);

  Serial.begin(57600);
  Serial.println("\nbooting");


  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {

}
