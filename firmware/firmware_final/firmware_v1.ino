#include <ESP8266WiFi.h>
#include <InfluxDbClient.h>
#include <InfluxDbCloud.h>
#include <Ticker.h>
#include <ArduinoWebsockets.h>

using namespace websockets;

// --- Configuración WiFi ---
const char* WIFI_SSID = "Fibertel WiFi565 2.4GHz";
const char* WIFI_PASSWORD = "0142785516";

// --- Configuración InfluxDB ---
#define INFLUXDB_URL "https://influxdb.gecep.ing.unlp.edu.ar/"
#define INFLUXDB_TOKEN "EldsDkPAlutR3EvFhh6ZkMiUuCAef-pTCjk166om7fKIHmxAF6tNC9fQrRHlQddKJOo7NeW3oEJhTZ217r2xmw=="
#define INFLUXDB_ORG "arsénico"
#define INFLUXDB_BUCKET "ORP sensor simulation"
#define TZ_INFO "UTC-3"
#define DEVICE "ESP8266"

// Instancia global del cliente InfluxDB
InfluxDBClient influxClient(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN, InfluxDbCloud2CACert);

// --- Simulación de datos ORP ---
// Ticker para disparar el envío de datos simulados cada 5 segundos
Ticker timer;
volatile bool sendDataFlag = false;
void timerCallback() {
  sendDataFlag = true;
}

// --- Configuración WebSocket ---
// El ESP8266 se conecta al backend para recibir alertas
WebsocketsClient wsClient;

// Callback del WebSocket: se llama al recibir un mensaje (alerta) del backend
void wsEventCallback(WebsocketsMessage message) {
  Serial.print("[WS] Alerta recibida: ");
  Serial.println(message.data());
  // Aquí puedes procesar el contenido, por ejemplo, encender un LED o activar una acción.
  // En este ejemplo, solo mostramos el mensaje recibido.
}

void setup() {
  Serial.begin(115200);
  Serial.println("\n[Setup] Iniciando programa...");
  Serial.setDebugOutput(true);

  // Inicializa la semilla aleatoria para la simulación
  randomSeed(analogRead(A0));
  
  // Conectar a WiFi
  Serial.print("[WiFi] Conectando a ");
  Serial.println(WIFI_SSID);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }
  Serial.println("\n[WiFi] Conectado. IP: " + WiFi.localIP().toString());
  
  // Sincronizar la hora vía NTP
  Serial.println("[TimeSync] Sincronizando hora...");
  timeSync(TZ_INFO, "pool.ntp.org", "time.nis.gov");
  Serial.println("[TimeSync] Hora sincronizada.");

  // Conexión a InfluxDB
  Serial.println("[InfluxDB] Validando conexión...");
  if (influxClient.validateConnection()) {
    Serial.print("[InfluxDB] Conectado a InfluxDB: ");
    Serial.println(influxClient.getServerUrl());
  } else {
    Serial.print("[InfluxDB] Conexión fallida: ");
    Serial.println(influxClient.getLastErrorMessage());
  }
  
  // Iniciar temporizador para enviar datos simulados cada 5 segundos
  Serial.println("[Simulación] Iniciando temporizador (cada 5 seg)...");
  timer.attach(5, timerCallback);
  
  // Conectar al servidor WebSocket (ajusta la IP según tu backend)
  String wsUrl = "ws://192.168.0.59:3500";
  Serial.print("[WS] Conectando a ");
  Serial.println(wsUrl);
  wsClient.onMessage(wsEventCallback);
  if (wsClient.connect(wsUrl)) {
    Serial.println("[WS] Conectado al servidor WebSocket");
  } else {
    Serial.println("[WS] Error al conectar al servidor WebSocket");
  }
}

void sendSimulatedData() {
  Serial.println("\n[Simulación] Preparando envío de datos simulados...");

  // Generar un valor aleatorio entre 300 y 700 mV
  long randomValue = random(300, 701);
  float sensorValue = (float)randomValue;

  // Crear un punto de datos para el sensor ORP (sensorORP_2)
  Point point("promedio_movil_ORP");
  point.addTag("device", DEVICE);
  point.addTag("sensor_id", "sensorORP_2");
  point.addField("orp_value", sensorValue);

  // Enviar datos a InfluxDB
  Serial.println("[InfluxDB] Enviando datos simulados...");
  bool writeSuccess = influxClient.writePoint(point);
  if (!writeSuccess) {
    Serial.print("[InfluxDB] Error al escribir datos: ");
    Serial.println(influxClient.getLastErrorMessage());
  } else {
    Serial.println("[InfluxDB] Datos simulados enviados exitosamente:");
    Serial.print("  SensorORP_2 ORP value (mV): ");
    Serial.println(sensorValue);
  }
}

void loop() {
  // Procesar mensajes del WebSocket (para recibir alertas del backend)
  wsClient.poll();
  
  // Si la bandera está activa, enviar datos simulados a InfluxDB
  if (sendDataFlag) {
    sendDataFlag = false;
    sendSimulatedData();
    delay(100);
  }
}
