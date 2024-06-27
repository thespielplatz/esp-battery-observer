#include <WiFiManager.h>
#include <Config.h>

class ExtendedWifiManager {
public:
  bool wifiManagerNonBlocking = true;

  void setup(std::vector<String> parameters_);
  void loop();
  String getValue(const String &key);

private:
  Config config;
  std::vector<String> parameters;
  WiFiManager wifiManager;
  std::vector<WiFiManagerParameter*> wifiManagerParameters;
  WiFiManagerParameter* testParameter;
  bool didAutoconnect = false;

  void loadConfig();
  void wifiManagerSaveConfigCallback();
};