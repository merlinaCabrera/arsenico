#include <ESP8266WiFi.h>

const char* ssid = "Fibertel WiFi565 2.4GHz";
const char* password = "0142785516";

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("\nWiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // Mantener el bucle vacío, ya que solo necesitas conectarte al Wi-Fi
}
