#include <ArduinoJson.h> 
#include <Observer.h>

// IMPLEMENTATION OF BATTERYMETER - START
#include "BatteryMeter.h"
const int batteryPin = 36;
const float R1 = 4710.0;
const float R2 = 10050.0;
const float vmin = 3.0;
const float vmax = 4.2;
BatteryMeter *batteryMeter = new BatteryMeter(R1, R2, batteryPin, vmin, vmax);
void printBatteryData(BatteryMeter::BatteryData data);
JsonDocument getBatteryData(BatteryMeter::BatteryData data);;
// IMPLEMENTATION OF BATTERYMETER - END

Observer *observer = new Observer();

void setup() {
  Serial.begin(115200);
  delay(1000); // Wait for serial monitor to open

  Serial.println("Starting...");
  observer->setup();
}

void loop() {
  // IMPLEMENTATION OF BATTERYMETER - START
  BatteryMeter::BatteryData data = batteryMeter->getBatteryData();
  printBatteryData(data);
  JsonDocument doc = getBatteryData(data);
  // IMPLEMENTATION OF BATTERYMETER - END

  observer->setPostBody(doc);
  observer->loop();
  delay(1000); // Update every second
}

// IMPLEMENTATION OF BATTERYMETER - START
JsonDocument getBatteryData(BatteryMeter::BatteryData data) {
  JsonDocument doc;
  doc["analogRead"] = data.analogRead;
  doc["pinVoltage"] = data.pinVoltage;
  doc["batteryVoltage"] = data.batteryVoltage;
  doc["percentage"] = data.percentage;
  return doc;
}

void printBatteryData(BatteryMeter::BatteryData data) {
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
// IMPLEMENTATION OF BATTERYMETER - END
