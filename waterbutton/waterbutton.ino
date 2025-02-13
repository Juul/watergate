
#include <WiFi.h>
#include <HTTPClient.h>

#include "settings.h"

int BUTTON_ON_PIN = SETTING_BUTTON_ON_PIN;
int BUTTON_OFF_PIN = SETTING_BUTTON_OFF_PIN;

const char WIFI_SSID[] = SETTING_WIFI_SSID; // change in settings.h
const char WIFI_PASSWORD[] = SETTING_WIFI_PASSWORD; // change in settings.h

String BASE_URL = String("http://") + SETTING_WATERGATE_HOSTNAME;

int watergate_request(int open) {
  HTTPClient http;

  if(open) {
    http.begin(BASE_URL + String("/sf/on/5"));
  } else { // send close request instead
    http.begin(BASE_URL + String("/sf/off"));
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
  Serial.println("\nbooting...");

  pinMode(BUTTON_ON_PIN, INPUT_PULLUP);
  pinMode(BUTTON_OFF_PIN, INPUT_PULLUP);

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

void pressed_on_button() {
  static unsigned long last_click = 0;
  
  if(millis() - last_click > DEBOUNCE_TIME) { 
    Serial.println("Pressed on button");
    watergate_request(1);
    last_click = millis();;
  }
}

void pressed_off_button() {
  static unsigned long last_click = 0;
  
  if(millis() - last_click > DEBOUNCE_TIME) {
    Serial.println("Pressed off button");
    watergate_request(0);
    last_click = millis();
  }
}

void loop() {
  int on_button, off_button;
  static int on_button_prev = 1;
  static int off_button_prev = 0;

  on_button = digitalRead(BUTTON_ON_PIN);
  off_button = digitalRead(BUTTON_OFF_PIN);

  if(on_button && !on_button_prev) {
    pressed_on_button();
  }

  if(!off_button && off_button_prev) {
    pressed_off_button();
  }

  /*
  Serial.print("on: ");
  Serial.print(on_button);
  Serial.print(on_button_prev);
  Serial.println("");

  Serial.print("off: ");
  Serial.print(off_button);
  Serial.print(on_button_prev);
  Serial.println("");
  */
  
  on_button_prev = on_button;
  off_button_prev = off_button;
  
  /*
  delay(5000);
  watergate_request(1);
  delay(2000);
  watergate_request(0);
  */
  delay(100);  
}
