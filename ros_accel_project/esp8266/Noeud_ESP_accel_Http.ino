#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>

const char* ssid = "A54 de Dabira";
const char* password = "dabira14";

const char* serverUrl = "http://10.108.156.90:5000/accel";

// Capteur ADXL345
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

void setup() {

  Serial.begin(115200);

  // WiFi
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connexion WiFi...");
  }

  Serial.println("WiFi OK");

  // ADXL345 init
  if (!accel.begin()) {
    Serial.println("ADXL345 non detecte !");
    while (1);
  }

  accel.setRange(ADXL345_RANGE_16_G);
}

void loop() {

  sensors_event_t event;
  accel.getEvent(&event);

  float x = event.acceleration.x;
  float y = event.acceleration.y;
  float z = event.acceleration.z;

  Serial.print("X: "); Serial.print(x);
  Serial.print(" Y: "); Serial.print(y);
  Serial.print(" Z: "); Serial.println(z);

  if (WiFi.status() == WL_CONNECTED) {

    WiFiClient client;
    HTTPClient http;

    http.begin(client, serverUrl);
    http.addHeader("Content-Type", "application/json");

    String data = "{";
    data += "\"x\":" + String(x) + ",";
    data += "\"y\":" + String(y) + ",";
    data += "\"z\":" + String(z);
    data += "}";

    int code = http.POST(data);

    Serial.println(code);

    http.end();
  }

  delay(100);
}