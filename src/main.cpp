#include <Arduino.h>
#ifdef ESP32
  #include <WiFi.h>
#else
  #include <ESP8266WiFi.h>
#endif
#include <PubSubClient.h>
#include <ArduinoJson.h>

// Local network login info
const char* ssid     = "House66-iot"; // enter SSID of WIFI router here
const char* password = "2zzFcVpePMkM"; // enter password to WIFI router here

// Add your MQTT Broker IP address, example:
const char* mqtt_server = "192.168.178.254";
const char* mqtt_user = "mqtt";
const char* mqtt_pass = "Welkom01";
const char* tmac = "BCDDC2250D2C";
const char* tsensor = "humi";



WiFiClient espClient;
PubSubClient mqtt(espClient);

void prepJson(const char *mac, const char *sensor) {
  char stat_t[40];
  char tname[40];
  char uniq_id[40];
  char buffer[300], topic[60];

  // Allocate the JSON document
  //
  // Inside the brackets, 300 is the RAM allocated to this document.
  // Don't forget to change this value to match your requirement.
  // Use arduinojson.org/v6/assistant to compute the capacity.
  StaticJsonDocument<300> doc;
  
  snprintf (stat_t, sizeof(stat_t), "EspNow/esp32/%s", mac );
  snprintf (tname,sizeof(tname), "ESP32 %s %s", mac, sensor);
  snprintf (uniq_id, sizeof(uniq_id), "esp32_%s_%s", mac, sensor);
  
  if (strcmp (sensor, "temp")==0){
    doc["unit_of_meas"] = "°C";
    doc["icon"] = "mdi:thermometer";
  }
  else if (strcmp (sensor, "pres")==0){
    doc["unit_of_meas"] = "hPa";
    doc["icon"] = "mdi:gauge";
  }
  else if (strcmp (sensor, "volt")==0){
    doc["unit_of_meas"] = "V";
    doc["icon"] = "mdi:flash";
  }
  else if (strcmp (sensor, "humi")==0) {
    doc["unit_of_meas"] = "%";
    doc["icon"] = "mdi:percent";
  }
  doc["stat_t"] = stat_t;
  doc["name"] = tname;
  doc["uniq_id"] = uniq_id;
  JsonObject device = doc.createNestedObject("device");
  device["ids"][0] = "ESP-NOW_Sensors";
  device["name"] = "ESP-NOW Sensors";
  device["model"] = "ESP32";
  device["mf"] = "MBD";
  
  serializeJson(doc, buffer);

  // prepare mqtt topic
  snprintf (topic, sizeof(topic),"homeassistant/sensor/esp32/%s_%s/config", tmac, tsensor);

  // publish buffer to mqtt server
  if (mqtt.publish(topic, buffer, true) == true) {
    Serial.println("Success sending message");
  } else {
    Serial.println("Error sending message");
  }
  Serial.println(buffer);
}

void setup() { 
  // Initialize Serial port
  Serial.begin(115200);
  while (!Serial) continue;

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode (WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected.");
  
  mqtt.setServer(mqtt_server, 1883);
  // Increase mqtt message size 
  mqtt.setBufferSize(512);

  // Setup MQTT Client
  while (!mqtt.connected()) {
    Serial.println("Connecting to MQTT...");
    // Attempt to connect
    if (mqtt.connect("ESP32Client", mqtt_user, mqtt_pass)) {
      Serial.println("connected");
    } else {
      Serial.print("failed with state ");
      Serial.print(mqtt.state());
      Serial.println();
      delay(2000);
    }
  }


  // Prepare JSON payload
  prepJson(tmac, tsensor);
  

}


void loop()
{
  mqtt.loop();

  delay(10000);

}