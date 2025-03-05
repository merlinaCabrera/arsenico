#include <ESP8266WiFiMulti.h>
ESP8266WiFiMulti wifiMulti;
#define DEVICE "ESP8266"
#include <InfluxDbClient.h>
#include <InfluxDbCloud.h>
#include <Ticker.h>

const char* WIFI_SSID = "Ubiquiti LEICI";     // Fibertel WiFi565 2.4GHz        Ubiquiti LEICI
const char* WIFI_PASSWORD = "leicidefiunlp";  // 0142785516                     leicidefiunlp

#define INFLUXDB_URL "https://influxdb.gecep.ar"
#define INFLUXDB_TOKEN "hw7oYzqkJlQbW2gW2b4BTQk7OH6Kf239lmrvX25owHtZcYMGMXa33s1y7nCLKmUZXqQ0QXL3XCONyKhvtbZWXw=="
#define INFLUXDB_ORG "d6bb789fb1484ab2"
#define INFLUXDB_BUCKET "pH sensor"
#define TZ_INFO "UTC-3"

#define LED_PIN_VERDE 12      // LED verde
#define LED_PIN_AMARILLO 13   // LED amarillo
#define LED_PIN 2             // Pin del LED interno del ESP8266 
#define ANALOG_PIN A0         // Pin analógico

// Declaración de cliente
InfluxDBClient client(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN, InfluxDbCloud2CACert);

// Variables
float voltage = 0.0;
float orpValue = 0.0;
float avgORP = 0.0;   // Valor ORP accesible en loop()

// Factor de calibracion
const float calibration = 0.1;

int readCount = 0; // Contador de lecturas

// Variables para el promedio móvil
const int totalReads = 10;       // Número de lecturas para el promedio móvil
float orpValues[totalReads];     // Almacenar las últimas lecturas
int readIndex = 0;               // Indice de la lectura actual
int numReadings = 0;             // Número de lecturas tomadas (hasta totalReads)
float sum = 0.0;                 // Suma de las lecturas actuales para el promedio

// Flag para indicar si se deben tomar lecturas
volatile bool readSensor = false;
bool shouldSendData = false; // Nueva bandera para enviar datos a InfluxDB

Ticker timer;                 // Instancia de Ticker
Ticker ledTimer;              // Instancia para controlar el apagado del LED

// Función para apagar el LED
void turnOffLed(int ledPin) {
  digitalWrite(ledPin, LOW); // Apaga el LED
}

// Leer el valor analógico y controlar el LED
void readAnalogSignal() {
  int sensorValue = analogRead(ANALOG_PIN); // Lee el valor del pin analógico (0-1023)

  Serial.print("Valor analógico: ");
  Serial.println(sensorValue);

  // Enciende el LED dependiendo del valor leído
  if (sensorValue > 512) { 
    digitalWrite(LED_PIN_VERDE, HIGH); // Enciende el LED verde
    Serial.println("LED verde");
    ledTimer.once(1, turnOffLed, LED_PIN_VERDE); // Apagar el LED verde después de 1 segundo
  } else {
    digitalWrite(LED_PIN_AMARILLO, HIGH); // Enciende el LED amarillo
    Serial.println("LED amarillo");
    ledTimer.once(1, turnOffLed, LED_PIN_AMARILLO); // Apagar el LED amarillo después de 1 segundo
  }

  // Convertir valor a voltaje (rango de entrada 0V - 1V)
  voltage = (sensorValue / 1023.0) * 1.0;

  // Convertir voltaje a rango de ORP (mV) ([0V, 1V] -> [-1000mV, 1000mV])
  orpValue = ((voltage - 0.66) *1000* 0.100/0.270); 

  // Restar la lectura más antigua de la suma
  sum -= orpValues[readIndex];

  // Almacenar la nueva lectura en el array
  orpValues[readIndex] = orpValue;

  // Añadir la nueva lectura a la suma
  sum += orpValues[readIndex];

  // Incrementar el índice circular
  readIndex = (readIndex + 1) % totalReads;

  // Actualizar el número de lecturas
  if (numReadings < totalReads) {
    numReadings++;
  }

  // Calcular el promedio móvil
  avgORP = sum / numReadings; // Actualizar avgORP

  // Mostrar los resultados
  Serial.print("Lectura cruda del sensor ORP: ");
  Serial.println(sensorValue);
  Serial.print("Voltaje medido: ");
  Serial.print(voltage, 4);
  Serial.println(" V");
  Serial.print("Valor ORP (mV): ");
  Serial.println(orpValue, 4);
  Serial.print("Promedio móvil ORP (mV): ");
  Serial.println(avgORP, 4);
  Serial.print("\n");
  Serial.print("Valores en el arreglo: ");

  for (int i = 0; i < totalReads; i++) {
    Serial.print(orpValues[i]);
    Serial.print(", ");
  }

  Serial.print("\n\n\n");

  // Preparar los datos para InfluxDB solo si numReadings >= 5
  if (numReadings >= 10) {
    // Aquí se declara y se crea el punto de datos cada vez que se necesita
    Point promedio_ORP("promedio_movil_ORP");
    promedio_ORP.addTag("device", DEVICE);
    promedio_ORP.addField("orp_value", avgORP); // Agrega el valor promedio ORP
    shouldSendData = true; // Indica que se deben enviar datos
  }
}

void setup() {
  Serial.begin(115200); // Comunicación serie
  // Setup wifi
  WiFi.mode(WIFI_STA);
  wifiMulti.addAP(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting to WiFi");
  while (wifiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }

  timeSync(TZ_INFO, "pool.ntp.org", "time.nis.gov");

  // Chequeo de conexión 
  if (client.validateConnection()) {
    Serial.print("Connected to InfluxDB: ");
    Serial.println(client.getServerUrl());
  } else {
    Serial.print("InfluxDB connection failed: ");
    Serial.println(client.getLastErrorMessage());
  }

  Serial.print("\n\n\n");

  pinMode(LED_PIN, OUTPUT);           // Pin del LED interno como salida
  pinMode(LED_PIN_VERDE, OUTPUT);     // Pin del LED verde como salida
  pinMode(LED_PIN_AMARILLO, OUTPUT);  // Pin del LED amarillo como salida

  // Inicializar el array de lecturas en 0
  for (int i = 0; i < totalReads; i++) {
    orpValues[i] = 0;
  }

  // Configurar el temporizador para llamar a la función cada 5 segundos
  timer.attach(5, readAnalogSignal);
}

void loop() {
  // Enviar datos a InfluxDB si la bandera está activa
  if (shouldSendData) {
    // Validar la conexión antes de intentar escribir
    if (client.validateConnection()) {
      // Escribir el punto de datos en InfluxDB
      Point promedio_ORP("promedio_movil_ORP"); // Crear el punto de datos aquí
      promedio_ORP.addTag("device", DEVICE);
      promedio_ORP.addField("orp_value", avgORP); // Agrega el valor promedio ORP

      if (!client.writePoint(promedio_ORP)) {
        Serial.print("Error writing to InfluxDB: ");
        Serial.println(client.getLastErrorMessage());
        delay(5000); // Espera antes de intentar nuevamente
      } else {
        Serial.println("Data written successfully!");

        // Imprimir los detalles del dato escrito
        Serial.print("Promedio móvil ORP (mV): ");
        Serial.println(avgORP); // Imprimir el promedio móvil
        Serial.print("Tag: device = ");
        Serial.println(DEVICE); // Imprimir el tag
        Serial.print("Field: orp_value = ");
        Serial.println(avgORP); // Imprimir el valor del campo
      }
    } else {
      Serial.println("InfluxDB connection failed.");
    }

    shouldSendData = false; // Reinicia flag
    // numReadings = 0; // Reinicia contador de lecturas
  }
}