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
  
  // WiFi AP SSID
  #define WIFI_SSID "Ubiquiti_LEICI"
  // WiFi password
  #define WIFI_PASSWORD "leicidefiunlp"
  
  #define INFLUXDB_URL "https://influxdb.gecep.ar"
  #define INFLUXDB_TOKEN "undefined"
  #define INFLUXDB_ORG "d6bb789fb1484ab2"
  #define INFLUXDB_BUCKET "pH sensor"
  
  // Time zone info
  #define TZ_INFO "UTC-3"
  
  // Declare InfluxDB client instance with preconfigured InfluxCloud certificate
  InfluxDBClient client(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN, InfluxDbCloud2CACert);
  
  // Declare Data point
  Point sensor("wifi_status");

  Point battery("battery_status");
  Point temperature("temperature_data");

  //politica de retencion para el almacenamiento de datos en un bucket
  //influx bucket create -n "nombre_del_bucket" -r 30d

  
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

    sensor.addTag("device", DEVICE);
    sensor.addTag("SSID", WiFi.SSID());
  }


void loop() {
    // Clear and update wifi_status data
  sensor.clearFields();
  sensor.addField("rssi", WiFi.RSSI());

  // Clear and update battery data
  battery.clearFields();
  battery.addField("voltage", 3.7); // Replace with actual battery voltage measurement

  // Clear and update temperature data
  temperature.clearFields();
  temperature.addField("value", 25.3); // Replace with actual temperature measurement

  // Send data to InfluxDB
  if (!client.writePoint(sensor)) {
    Serial.print("InfluxDB write failed for wifi_status: ");
    Serial.println(client.getLastErrorMessage());
  }
  if (!client.writePoint(battery)) {
    Serial.print("InfluxDB write failed for battery_status: ");
    Serial.println(client.getLastErrorMessage());
  }
  if (!client.writePoint(temperature)) {
    Serial.print("InfluxDB write failed for temperature_data: ");
    Serial.println(client.getLastErrorMessage());
  }

    // Imprimir lo que se va a enviar
    Serial.print("Writing: ");
    Serial.println(sensor.toLineProtocol());

  
    // Check WiFi connection and reconnect if needed
    if (wifiMulti.run() != WL_CONNECTED) {
      Serial.println("Wifi connection lost");
    }
  
  
    Serial.println("Waiting 1 second");
    delay(1000);
    }
