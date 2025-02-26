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
  
  void setup() {
    Serial.begin(9600);
  
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
    // ... code from Write Data step 
    
    // Query will find the RSSI values for last minute for each connected WiFi network with this device
     String query = "from(bucket: \"pH sensor\")\n\
   |> range(start: -1m)\n\
   |> filter(fn: (r) => r._measurement == \"wifi_status\" and r._field == \"rssi\")";
   
     // Print composed query
     Serial.println("Querying for RSSI values written to the \"pH sensor\" bucket in the last 1 min... ");
     Serial.println(query);
   
     // Send query to the server and get result
     FluxQueryResult result = client.query(query);
   
     Serial.println("Results : ");
     // Iterate over rows.
     while (result.next()) {
       // Get converted value for flux result column 'SSID'
       String ssid = result.getValueByName("SSID").getString();
       Serial.print("SSID '");
       Serial.print(ssid);
   
       Serial.print("' with RSSI ");
       // Get value of column named '_value'
       long value = result.getValueByName("_value").getLong();
       Serial.print(value);
   
       // Get value for the _time column
       FluxDateTime time = result.getValueByName("_time").getDateTime();
   
       String timeStr = time.format("%F %T");
   
       Serial.print(" at ");
       Serial.print(timeStr);
   
       Serial.println();
     }
   
     // Report any error
     if (result.getError() != "") {
       Serial.print("Query result error: ");
       Serial.println(result.getError());
     }
   
     // Close the result
     result.close();
   
     Serial.println("==========");
   
     delay(5000);
   }