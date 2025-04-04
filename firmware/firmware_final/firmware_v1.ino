/* Esta versión integra la lectura del sensor ORP (real o simulada),
conexión a WiFi y a InfluxDB, envío de los datos a la base de datos y
la posterior recepción de alertas del backend por Web Sockets */

#include <ESP8266WiFi.h>
#include <InfluxDbClient.h>
#include <InfluxDbCloud.h>
#include <Ticker.h>
#include <ArduinoWebsockets.h>

using namespace websockets;

// Credenciales WiFi
/* 
// LEICI
const char* WIFI_SSID = "";
const char* WIFI_PASSWORD = "";
*/

// Merlina
const char* WIFI_SSID = "Fibertel WiFi565 2.4GHz";
const char* WIFI_PASSWORD = "0142785516";

// Datos de InfluxDB
#define INFLUXDB_URL "https://influxdb.gecep.ing.unlp.edu.ar/"
#define INFLUXDB_TOKEN "EldsDkPAlutR3EvFhh6ZkMiUuCAef-pTCjk166om7fKIHmxAF6tNC9fQrRHlQddKJOo7NeW3oEJhTZ217r2xmw=="  
#define INFLUXDB_ORG "arsénico"          
#define INFLUXDB_BUCKET "ORP sensor simulation"
#define TZ_INFO "UTC-3"

#define DEVICE "ESP8266"

// Instancia del cliente InfluxDB 
InfluxDBClient client(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN, InfluxDbCloud2CACert);


// ----------------------------------------------------------------------
// LECTURA DE ORP SIMULADA

// Objeto Ticker para enviar datos simulados cada 5 s
Ticker timer;

// Flag para indicar que hay datos para enviar
volatile bool sendDataFlag = false;

// Callback del Ticker: Establece flag para enviar datos
void ICACHE_FLASH_ATTR timerCallback() {
  sendDataFlag = true;
}

// **WebSocket**
WebsocketsClient wsClient;

// Callback de WebSocket: Se llama al recibir mensajes desde el backend
void wsEventCallback(WebsocketsMessage message) {
  Serial.print("[WS] Mensaje recibido: ");
  Serial.println(message.data());
  // Aquí podrías analizar el mensaje (por ejemplo, parsear JSON) y tomar decisiones,
  // como activar el envío inmediato de datos o controlar la válvula.
  // Por ejemplo, si el mensaje es "ENVIAR_DATOS", puedes forzar sendDataFlag=true;
  if (String(message.data()) == "ENVIAR_DATOS") {
    sendDataFlag = true;
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("\n[Setup] Iniciando programa...");
  Serial.setDebugOutput(true);

  // Inicializa semilla aleatoria para números aleatorios
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
  if (influxClient.validateConnection()) {
    Serial.print("[InfluxDB] Conectado a InfluxDB: ");
    Serial.println(influxClient.getServerUrl());
  } else {
    Serial.print("[InfluxDB] Conexión fallida: ");
    Serial.println(influxClient.getLastErrorMessage());
  }
  
  // Iniciar el temporizador para solicitar envío de datos cada 5 segundos
  Serial.println("[Simulación] Iniciando temporizador para enviar datos cada 5 segundos...");
  timer.attach(5, timerCallback);
  
  // Conectar al servidor WebSocket (ajusta la IP y el puerto según tu backend)
  String wsUrl = "ws://192.168.0.59:3500";
  Serial.print("[WS] Conectando a ");
  Serial.println(wsUrl);
  wsClient.onMessage(wsEventCallback);
  bool wsConnected = wsClient.connect(wsUrl);
  if (wsConnected) {
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

  // Crear punto de datos para el sensor ORP_2
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
  // Procesa mensajes de WebSocket
  wsClient.poll();
  
  // Si la bandera está activada, enviar los datos y resetear la bandera
  if (sendDataFlag) {
    sendDataFlag = false;
    sendSimulatedData();
    delay(100); // Pequeño delay para evitar saturar el loop
  }
}


// ----------------------------------------------------------------------
// LECTURA DE ORP REAL

/*


*/


/* A continuación...
Manejo de driver de motor paso a paso & caudalímetro */
