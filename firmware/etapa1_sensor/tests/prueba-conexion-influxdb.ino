#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h> 

const char* ssid = "Fibertel WiFi565 2.4GHz";
const char* password = "0142785516";
const char* server = "influxdb.gecep.ar"; // Ajusta esta dirección si es necesario
const int port = 443; // Cambia a 443 para HTTPS

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("\nWiFi connected");
  
  // Intentar conectar al servidor usando WiFiClientSecure
  WiFiClientSecure client;
  client.setInsecure(); // Para pruebas iniciales, elimina en producción.
  
  Serial.print("Connecting to InfluxDB server...");
  if (client.connect(server, port)) {
    Serial.println("connected");
    client.stop();
  } else {
    Serial.println("connection failed");
  }
}

void loop() {
  // Mantener el bucle vacío, ya que solo necesitas una prueba de conexión
}
