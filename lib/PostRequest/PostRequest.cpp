#include "PostRequest.h"
#include <HTTPClient.h>

void PostRequest::setPostBody(JsonDocument &json) {
  this->json = &json;
}

void PostRequest::sendRequest(String url) {
  lastHttpResponseCode = -1;

  Serial.print("Sending POST request to: ");
  Serial.println(url);

  HTTPClient http;
  http.begin(url);
  http.addHeader("Content-Type", "application/json");

  String requestBody;
  serializeJson(*json, requestBody);
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
