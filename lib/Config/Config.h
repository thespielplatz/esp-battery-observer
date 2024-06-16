#include "WString.h"
#include <ArduinoJson.h> 

class Config {
public:
  void loadConfigFile(const String& filename);
  void saveConfigFile(const String& filename);
  void setValue(const String& key, const String& value);
  String getValue(const String& key);

private:
  JsonDocument options;
};