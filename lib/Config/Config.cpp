#include "Config.h"
#include <FS.h> //this needs to be first, or it all crashes and burns...
#ifdef ESP32
  #include <SPIFFS.h>
#endif

void Config::loadConfigFile(const String& filename) {
  if (!SPIFFS.begin(true)) { // mounts the filesystem and formats it if it is not formatted
    Serial.println("failed to mount FS");    
    return;
  }
  if (!SPIFFS.exists(filename)) {
    Serial.println("Config file does not exist, using default configuration");
    return;
  }

  File configFile = SPIFFS.open(filename, "r");
  if (!configFile) {
    Serial.println("Failed to open config file");
    return;
  }

  Serial.print("opened config file");
  Serial.println(filename);

  DeserializationError error = deserializeJson(options, configFile);
  configFile.close();

  if (error) {
    Serial.println("failed to parse json");
    Serial.println(error.c_str());    
  } else {
    Serial.println("parsed json");
    serializeJson(options, Serial);
  }
}

void Config::saveConfigFile(const String& filename) {
  if (!SPIFFS.begin()) {
    Serial.println("failed to mount FS");
    return;
  }

  File configFile = SPIFFS.open(filename, "w");
  if (!configFile) {
    Serial.println("Failed to open config file for writing");
    return;
  }
  serializeJson(options, Serial);
  int result = serializeJson(options, configFile);
  if (result == 0) {
    Serial.println(F("Failed to write to file"));
  }

  configFile.close();
}

void Config::setValue(const String& key, const String& value) {
  options[key] = value;
}

String Config::getValue(const String& key) {
  if (options[key] == nullptr) {
    return "";
  }
  return options[key];
}