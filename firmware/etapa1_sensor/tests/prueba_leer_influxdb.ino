#include <ESP8266WiFiMulti.h>
ESP8266WiFiMulti wifiMulti;

#include <InfluxDbClient.h>
#include <InfluxDbCloud.h>

const char* WIFI_SSID = "Fibertel WiFi565 2.4GHz"; // Fibertel WiFi565 2.4GHz        Ubiquiti LEICI
const char* WIFI_PASSWORD = "0142785516"; // 0142785516                     leicidefiunlp

// token
#define INFLUXDB_TOKEN "6ixkc7rOqy8FOvN4ARRHjbRx7kJwRwxkswsPDvzwXAQC8rYoz930nN0UYaTrfaOQkAibZ31GJNa5RM-j9pL0fg=="

#define INFLUXDB_URL "https://influxdb.gecep.ar"
#define INFLUXDB_ORG "d6bb789fb1484ab2"
#define INFLUXDB_BUCKET "pH sensor"
#define TZ_INFO "UTC-3"

// Declarar el cliente InfluxDB
InfluxDBClient client(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN, InfluxDbCloud2CACert);

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  wifiMulti.addAP(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting to WiFi");
  while (wifiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }
  Serial.println("\nWiFi connected!");

  // Sincronizar la hora para la validación de certificados
  timeSync(TZ_INFO, "pool.ntp.org", "time.nis.gov");

  // Verificar conexión con InfluxDB
  if (client.validateConnection()) {
    Serial.print("Connected to InfluxDB: ");
    Serial.println(client.getServerUrl());
  } else {
    Serial.print("InfluxDB connection failed: ");
    Serial.println(client.getLastErrorMessage());
  }
}

void readDataFromInfluxDB() {
  // Consultar los últimos datos escritos
  String query = "from(bucket: \"" + String(INFLUXDB_BUCKET) + "\") |> range(start: -1h) |> filter(fn: (r) => r._measurement == \"promedio_movil_ORP\")";

  FluxQueryResult result = client.query(query);
  
  if (result.next()) {
    // Obtener los datos de la respuesta
    String measurement = result.getValueByName("_measurement").getString();
    String field = result.getValueByName("_field").getString();
    float value = result.getValueByName("_value").getDouble();

    // Mostrar los datos leídos
    Serial.print("Medida: ");
    Serial.println(measurement);
    Serial.print("Campo: ");
    Serial.println(field);
    Serial.print("Valor: ");
    Serial.println(value);
  } else {
    Serial.println("No data available or query failed.");
  }

  // Manejar errores
  if (result.getError() != "") {
    Serial.print("Query error: ");
    Serial.println(result.getError());
  }

  result.close();  // Cerrar el resultado de la consulta
}

void loop() {
  readDataFromInfluxDB();
  delay(10000); // Esperar 10 segundos antes de la próxima consulta
}
