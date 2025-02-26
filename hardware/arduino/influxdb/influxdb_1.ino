#if defined(ESP32)
  #include <WiFiMulti.h>
  WiFiMulti wifiMulti;
  #define DEVICE "ESP32"
  #elif defined(ESP8266)
  #include <ESP8266WiFiMulti.h>
  ESP8266WiFiMulti wifiMulti;
  #define DEVICE "ESP8266"
  #endif
  
#include <InfluxDbClient.h>
#include <InfluxDbCloud.h>

const char* WIFI_SSID = "Fibertel WiFi565 2.4GHz";
const char* WIFI_PASSWORD = "0142785516";

#define INFLUXDB_URL "https://influxdb.gecep.ar"
#define INFLUXDB_TOKEN "hw7oYzqkJlQbW2gW2b4BTQk7OH6Kf239lmrvX25owHtZcYMGMXa33s1y7nCLKmUZXqQ0QXL3XCONyKhvtbZWXw==" // TOKEN !!!!!!!!!!
#define INFLUXDB_ORG "d6bb789fb1484ab2"
#define INFLUXDB_BUCKET "pH sensor"

// Time zone info
#define TZ_INFO "UTC-3"

// Declare InfluxDB client instance with preconfigured InfluxCloud certificate
InfluxDBClient client(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN, InfluxDbCloud2CACert);

// Declare Data point
Point sensor("wifi_status");

void setup() {
    Serial.begin(115200);

    // Setup wifi
    WiFi.mode(WIFI_STA);
    wifiMulti.addAP(WIFI_SSID, WIFI_PASSWORD);

    Serial.print("Connecting to wifi");
    while (wifiMulti.run() != WL_CONNECTED) {
        Serial.print(".");
        delay(100);
    }
    Serial.println();

    // Accurate time is necessary for certificate validation and writing in batches
    // We use the NTP servers in your area as provided by: https://www.pool.ntp.org/zone/
    // Syncing progress and the time will be printed to Serial.
    timeSync(TZ_INFO, "pool.ntp.org", "time.nis.gov");

    // Check server connection
    if (client.validateConnection()) {
        Serial.print("Connected to InfluxDB: ");
        Serial.println(client.getServerUrl());
    } else {
        Serial.print("InfluxDB connection failed: ");
        Serial.println(client.getLastErrorMessage());
    }
}

void loop() {
    // Agregar información al punto de datos
    sensor.addTag("device", DEVICE); // Agrega una etiqueta con el nombre del dispositivo
    sensor.addField("rssi", WiFi.RSSI()); // Agrega el valor RSSI
    sensor.addField("other_metric", 123.45); // Agrega otro campo como ejemplo
    float orpValue = 138.0;
    sensor.addField("orp_value", orpValue); // Agrega el valor ORP


    // Escribir el punto de datos en InfluxDB
    if (!client.writePoint(sensor)) {
        Serial.print("Error writing to InfluxDB: ");
        Serial.println(client.getLastErrorMessage());
    } else {
        Serial.println("Data written successfully!");
    }

    // Limpiar el punto de datos para la próxima escritura
    sensor.clearFields(); 

    // Esperar 10 segundos antes de la próxima escritura
    delay(10000);
}