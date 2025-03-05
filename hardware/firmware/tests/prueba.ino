int const buttonPin = 14;       // Botón asignado en el pin D5
#define LED_PIN_VERDE 12      // LED verde
#define LED_PIN_AMARILLO 13   // LED amarillo
#define LED_PIN 2             // Pin del LED interno del ESP8266 
#define ANALOG_PIN A0         // Pin analógico

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




void ICACHE_RAM_ATTR onButtonPress(void);


void setup() {
  // Vamos a usar el puerto serie para mostrar el estado del botón.
  Serial.begin(115200);

  pinMode(buttonPin,INPUT);
  pinMode(LED_PIN, OUTPUT);           // Pin del LED interno como salida
  pinMode(LED_PIN_VERDE, OUTPUT);     // Pin del LED verde como salida
  pinMode(LED_PIN_AMARILLO, OUTPUT);  // Pin del LED amarillo como salida

  // Inicializar el array de lecturas en 0
  for (int i = 0; i < totalReads; i++) {
    orpValues[i] = 0;
  }
  // Configurar la interrupción para detectar el flanco de bajada en el botón
  attachInterrupt(digitalPinToInterrupt(buttonPin), onButtonPress, FALLING); 
}

void loop() {
  // Verifica si se ha presionado el botón
  if (readSensor) {
    // Restablece la señal de lectura
    readSensor = false; 
  }
}

// Función de interrupción (ISR) para manejar el botón
void ICACHE_RAM_ATTR onButtonPress() {
  // Se debe leer el sensor
  readSensor = true; 
  Serial.println("Se presionó el botón");

  int sensorValue = analogRead(ANALOG_PIN); // Lee el valor del pin analógico (0-1023)
  Serial.print("Valor analógico: ");
  Serial.println(sensorValue);
    // Convertir valor a voltaje (rango de entrada 0V - 1V)
  voltage = (sensorValue / 1023.0) * 1.0;
  Serial.print("rango 0V,1V = ");
  Serial.print(voltage);
  Serial.print("\n");
  // Convertir voltaje a rango de ORP (mV) ([0V, 1V] -> [-1000mV, 1000mV]])
  orpValue = (voltage * 2000) - 1000; 
  Serial.print("rango -1000mV,1000mV = ");
  Serial.print(orpValue);
  Serial.print("\n\n\n"); 
}

