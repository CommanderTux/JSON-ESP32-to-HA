#include <Arduino.h>
#include <ArduinoJson.h>

void prepJson(const char *mac, const char *sensor, const char *unit) {
  char stat_t[40];
  char tname[40];
  char uniq_id[40];
  
  // Allocate the JSON document
  //
  // Inside the brackets, 300 is the RAM allocated to this document.
  // Don't forget to change this value to match your requirement.
  // Use arduinojson.org/v6/assistant to compute the capacity.
  StaticJsonDocument<300> doc;
  
  snprintf (stat_t, sizeof(stat_t), "EspNow/esp32/%s", mac );
  snprintf (tname,sizeof(tname), "ESP32 %s %s", mac, sensor);
  snprintf (uniq_id, sizeof(uniq_id), "esp32_%s_%s", mac, sensor);

  
  doc["unit_of_meas"] = unit;
  doc["icon"] = "mdi:flash";
  doc["stat_t"] = stat_t;
  doc["name"] = tname;
  doc["uniq_id"] = uniq_id;
  JsonObject device = doc.createNestedObject("device");
  device["ids"][0] = "ESP-NOW_Sensors";
  device["name"] = "ESP-NOW Sensors";
  device["model"] = "ESP32";
  device["mf"] = "MBD";
  
  char buffer[300];
  serializeJson(doc, buffer);
  return std::move(buffer);
}