#include "Observer.h"
#include <HTTPClient.h>

const String configFile = "/observer-config.json";
const String postUrlKey = "posturl";

Observer::Observer() {
  Observer(300000);
}

Observer::Observer(long intervalinSeconds) {
  interval = intervalinSeconds;
}

void Observer::setup() {
  loadConfig();

  WiFi.mode(WIFI_AP_STA);

  String postUrlValue = config.getValue(postUrlKey);
  this->postUrlParameter = new WiFiManagerParameter("posturl", "Post URL", postUrlValue.c_str(), 250);

  wifiManager.setSaveParamsCallback([this]() { this->wifiManagerSaveConfigCallback(); });
  wifiManager.addParameter(this->postUrlParameter);

  if (wifiManagerNonBlocking) wifiManager.setConfigPortalBlocking(false);

  didAutoconnect = wifiManager.autoConnect();
  if (didAutoconnect) {
      Serial.println("autoConnect...yeey :)");
      wifiManager.startWebPortal();
  } else {
      Serial.println("Failed to autoConnect");
  }

  previousMillis = millis() - interval; 
}

void Observer::setPostBody(JsonDocument &json) {
  this->json = &json;
}

void Observer::loop() {
  loopWifiManager();
  loopSendData();
}

void Observer::loadConfig() {
  Serial.println("Load Config");
  config.loadConfigFile(configFile);
}

void Observer::loopWifiManager() {
  if (wifiManagerNonBlocking) wifiManager.process(); // avoid delays() in loop when non-blocking and other long running code  
}


void Observer::loopSendData() {
  if (this->postUrlParameter->getValue() == nullptr || strcmp(this->postUrlParameter->getValue(), "") == 0) {
    Serial.println("No post url set - skipping sending data");
    return;
  }

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Not connected to WiFi - skipping sending data");
    return;
  }

  unsigned long currentMillis = millis();

  // Check if 5 minutes have passed
  if (currentMillis - previousMillis >= interval) {
    sendRequest();

    if (lastHttpResponseCode == 200) {
      previousMillis = currentMillis;
    } else {
      previousMillis = currentMillis - interval + 5000; // Try again in 5 seconds
    }
  }
}

void Observer::sendRequest() {
  Serial.print("Sending POST request to: ");
  Serial.println(this->postUrlParameter->getValue());

  HTTPClient http;
  http.begin(this->postUrlParameter->getValue());
  http.addHeader("Content-Type", "application/json");

  String requestBody;
  serializeJson(*(this->json), requestBody);
  lastHttpResponseCode = http.POST(requestBody);

  if (lastHttpResponseCode > 0) {
    String response = http.getString();
    Serial.println("HTTP Response code: " + String(lastHttpResponseCode));
    Serial.println("Response: " + response);
  } else {
    Serial.println("Error on sending POST: " + String(lastHttpResponseCode));
  }

  http.end();
}

void Observer::wifiManagerSaveConfigCallback() {
  Serial.println("Save Config");
  config.setValue(postUrlKey, this->postUrlParameter->getValue());
  config.saveConfigFile(configFile);
}
