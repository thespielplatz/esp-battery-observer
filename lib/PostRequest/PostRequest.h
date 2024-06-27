#include <ArduinoJson.h>

class PostRequest {
public:
  int lastHttpResponseCode;

  void setPostBody(JsonDocument &json);
  void sendRequest(String url);

private:
  JsonDocument *json;
};