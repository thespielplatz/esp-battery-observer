#include <ArduinoJson.h> 
#include <ExtendedWifiManager.h>
#include <esp_timer.h>
#include <PostRequest.h>

const String postUrlKey = "postUrl";
const int postRequestTimeInSeconds = 300; // 5 minutes
PostRequest postRequest;

esp_timer_handle_t postRequestTimer;
bool sendPostRequestFlag = false;

// IMPLEMENTATION OF BATTERYMETER - START
#include "BatteryMeter.h"
esp_timer_handle_t printBatteryDataTimer;
bool printBatteryDataFlag = false;
const int batteryPin = 36;
const float R1 = 4710.0;
const float R2 = 10050.0;
const float vmin = 3.0;
const float vmax = 4.2;
BatteryMeter *batteryMeter = new BatteryMeter(R1, R2, batteryPin, vmin, vmax);
void loopPrintBatteryData();
JsonDocument getBatteryData(BatteryMeter::BatteryData data);
// IMPLEMENTATION OF BATTERYMETER - END

void loopSendBatteryData();

ExtendedWifiManager *extendedWifiManager = new ExtendedWifiManager();

void setup() {
  Serial.begin(115200);
  delay(1000); // Wait for serial monitor to open

  Serial.println("Starting...");

  extendedWifiManager->setup({ postUrlKey, "test" });

  // Initialize and start the postRequest timer
  const esp_timer_create_args_t postRequestTimerArgs = {
    .callback = [](void* arg){ sendPostRequestFlag = true; },
    .name = "postRequestTimer"
  };
  esp_timer_create(&postRequestTimerArgs, &postRequestTimer);
  esp_timer_start_once(postRequestTimer, 1000000); // Start once initially after 1 second

  // IMPLEMENTATION OF BATTERYMETER - START
  // Initialize and start the printBatteryData timer
  const esp_timer_create_args_t printBatteryDataTimerArgs = {
    .callback = [](void* arg){ printBatteryDataFlag = true; },
    .name = "printBatteryDataTimer"
  };
  esp_timer_create(&printBatteryDataTimerArgs, &printBatteryDataTimer);
  esp_timer_start_periodic(printBatteryDataTimer, 1000000); // 1 second interval
  // IMPLEMENTATION OF BATTERYMETER - END
}

void loop() {
  extendedWifiManager->loop();
  loopSendBatteryData();
  // IMPLEMENTATION OF BATTERYMETER - START
  loopPrintBatteryData();
  // IMPLEMENTATION OF BATTERYMETER - END
}

// IMPLEMENTATION OF BATTERYMETER - START
void loopPrintBatteryData() {
  if (!printBatteryDataFlag) {
    return;
  }
  printBatteryDataFlag = false;
  BatteryMeter::BatteryData data = batteryMeter->getBatteryData();
  Serial.print("Analog Read: ");
  Serial.print(data.analogRead);
  Serial.print(", Pin Voltage: ");
  Serial.print(data.pinVoltage);
  Serial.print(" V, Battery Voltage: ");
  Serial.print(data.batteryVoltage);
  Serial.print(" V, Battery Percentage: ");
  Serial.print(data.percentage);
  Serial.println(" %");
}

JsonDocument getBatteryData(BatteryMeter::BatteryData data) {
  JsonDocument doc;
  doc["analogRead"] = data.analogRead;
  doc["pinVoltage"] = data.pinVoltage;
  doc["batteryVoltage"] = data.batteryVoltage;
  doc["percentage"] = data.percentage;
  return doc;
}
// IMPLEMENTATION OF BATTERYMETER - END

void loopSendBatteryData() {
  if (!sendPostRequestFlag) {
    return;
  }
  sendPostRequestFlag = false;

  // IMPLEMENTATION OF BATTERYMETER - START
  esp_timer_stop(printBatteryDataTimer); // Stop the printBatteryData timer
  BatteryMeter::BatteryData data = batteryMeter->getBatteryData();
  JsonDocument doc = getBatteryData(data);
  postRequest.setPostBody(doc);
  // IMPLEMENTATION OF BATTERYMETER - END
  postRequest.sendRequest(extendedWifiManager->getValue(postUrlKey));
  if (postRequest.lastHttpResponseCode == 200) {
    Serial.println("Battery data sent successfully.");
    esp_timer_start_once(postRequestTimer, postRequestTimeInSeconds * 1000000); // 300 seconds interval
  } else {
    Serial.println("Failed to send battery data.");
    esp_timer_start_once(postRequestTimer, 10000000); // Retry after 10 seconds
  }
  // IMPLEMENTATION OF BATTERYMETER - START
  esp_timer_start_periodic(printBatteryDataTimer, 1000000); // 1 second interval
  // IMPLEMENTATION OF BATTERYMETER - END
}
