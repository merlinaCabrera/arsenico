#include <ESP8266WiFi.h>
#include <InfluxDbClient.h>
#include <InfluxDbCloud.h>
#include <Ticker.h>

// Credenciales WiFi
const char* WIFI_SSID = "Fibertel WiFi565 2.4GHz";
const char* WIFI_PASSWORD = "0142785516";

// Datos de InfluxDB
#define INFLUXDB_URL "https://influxdb.gecep.ing.unlp.edu.ar/"
#define INFLUXDB_TOKEN "EldsDkPAlutR3EvFhh6ZkMiUuCAef-pTCjk166om7fKIHmxAF6tNC9fQrRHlQddKJOo7NeW3oEJhTZ217r2xmw=="  
#define INFLUXDB_ORG "arsénico"          
#define INFLUXDB_BUCKET "ORP sensor simulation"
#define TZ_INFO "UTC-3"

#define DEVICE "ESP8266"

// Instancia del cliente InfluxDB (usando certificado CA para InfluxDbCloud)
InfluxDBClient client(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN, InfluxDbCloud2CACert);

// Objeto Ticker
Ticker timer;

// Bandera para indicar que hay datos para enviar
volatile bool sendDataFlag = false;

// Callback del Ticker: Sólo establece la bandera
void ICACHE_FLASH_ATTR timerCallback() {
  sendDataFlag = true;
}

// Función para enviar datos de un solo sensor
void sendSimulatedData() {
  Serial.println("\n[Simulación] Preparando envío de datos simulados...");

  // Generar un valor aleatorio entre 300 y 700 mV
  long randomValue = random(300, 701);
  float sensorValue = (float)randomValue;

  // Crear punto de datos para el sensor ORP_1
  Point point("promedio_movil_ORP");
  point.addTag("device", DEVICE);
  point.addTag("sensor_id", "sensorORP_2");
  point.addField("orp_value", sensorValue);

  // Enviar datos a InfluxDB
  Serial.println("[InfluxDB] Enviando datos simulados...");
  bool write = client.writePoint(point);

  if (!write) {
    Serial.print("[InfluxDB] Error al escribir datos: ");
    Serial.println(client.getLastErrorMessage());
  } else {
    Serial.println("[InfluxDB] Datos simulados enviados exitosamente:");
    Serial.print("  SensorORP_2 ORP value (mV): ");
    Serial.println(sensorValue);
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("\n[Setup] Iniciando programa...");
  Serial.setDebugOutput(true);

  // Inicializar semilla aleatoria para números aleatorios
  randomSeed(analogRead(A0));
  
  // Configurar WiFi
  Serial.print("[WiFi] Conectando a ");
  Serial.println(WIFI_SSID);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }
  Serial.println("\n[WiFi] Conectado exitosamente a la red.");
  
  // Sincronizar el tiempo
  Serial.println("[TimeSync] Sincronizando hora...");
  timeSync(TZ_INFO, "pool.ntp.org", "time.nis.gov");
  Serial.println("[TimeSync] Hora sincronizada.");

  // Verificar conexión con InfluxDB
  Serial.println("[InfluxDB] Validando conexión...");
  if (client.validateConnection()) {
    Serial.print("[InfluxDB] Conectado a InfluxDB: ");
    Serial.println(client.getServerUrl());
  } else {
    Serial.print("[InfluxDB] Conexión fallida: ");
    Serial.println(client.getLastErrorMessage());
  }
  
  // Iniciar el temporizador para solicitar envío de datos cada 5 segundos
  Serial.println("[Simulación] Iniciando temporizador para enviar datos cada 5 segundos...");
  timer.attach(5, timerCallback);
}

void loop() {
  // Si la bandera está activada, enviar los datos y resetear la bandera
  if (sendDataFlag) {
    sendDataFlag = false;
    sendSimulatedData();
    // Se puede agregar un pequeño delay para evitar saturar el loop
    delay(100);
  }
}
