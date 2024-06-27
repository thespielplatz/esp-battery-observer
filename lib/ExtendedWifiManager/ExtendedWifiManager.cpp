#include "ExtendedWifiManager.h"
#include <HTTPClient.h>

const String configFile = "/observer-config.json";

void ExtendedWifiManager::setup(std::vector<String> parameters_) {
  parameters = parameters_;
  loadConfig();

  WiFi.mode(WIFI_AP_STA);

  wifiManager.setSaveParamsCallback([this]() { this->wifiManagerSaveConfigCallback(); });

  for (int i = 0; i < parameters.size(); i++) {
    String value = config.getValue(parameters[i]);
    WiFiManagerParameter *wiFiManagerParameter = new WiFiManagerParameter((parameters[i]).c_str(), parameters[i].c_str(), value.c_str(), 250);
    wifiManagerParameters.push_back(wiFiManagerParameter);
    wifiManager.addParameter(wiFiManagerParameter);
  }

  if (wifiManagerNonBlocking) wifiManager.setConfigPortalBlocking(false);

  didAutoconnect = wifiManager.autoConnect();
  if (didAutoconnect) {
      Serial.println("autoConnect...yeey :)");
      wifiManager.startConfigPortal();
  } else {
      Serial.println("Failed to autoConnect");
  }
}

void ExtendedWifiManager::loop() {
  if (wifiManagerNonBlocking) wifiManager.process(); // avoid delays() in loop when non-blocking and other long running code  
}

void ExtendedWifiManager::loadConfig() {
  Serial.println("Load Config");
  config.loadConfigFile(configFile);
}

void ExtendedWifiManager::wifiManagerSaveConfigCallback() {
  Serial.println("Save Config");
  for (int i = 0; i < parameters.size(); i++) {
    config.setValue(parameters[i], wifiManagerParameters[i]->getValue());
  }
  config.saveConfigFile(configFile);
}

String ExtendedWifiManager::getValue(const String &key) {
  return config.getValue(key);
}
