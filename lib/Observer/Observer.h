#include <WiFiManager.h>
#include <Config.h>

class Observer {
public:
  bool wifiManagerNonBlocking = true;
  int lastHttpResponseCode = 0;

  Observer();
  Observer(long intervalinSeconds);

  void setup();
  void setPostBody(JsonDocument &json);
  void loop();

private:
  Config config;
  WiFiManager wifiManager;
  WiFiManagerParameter *postUrlParameter;
  bool didAutoconnect = false;
  unsigned long previousMillis = 0; // Stores the last time the POST request was sent
  long interval;
  JsonDocument *json;

  void loadConfig();
  void wifiManagerSaveConfigCallback();
  void loopWifiManager();
  void loopSendData();
  void sendRequest();
};