#include <Ticker.h>

#define LED_PIN_VERDE 12      // LED verde
#define LED_PIN_AMARILLO 13   // LED amarillo
#define LED_PIN 2             // Pin del LED interno del ESP8266 
#define ANALOG_PIN A0         // Pin analógico


// Variables
float voltage = 0.0;
float orpValue = 0.0;

// Factor de calibracion
const float calibration = 0.1;

int readCount = 0; // Contador de lecturas
const int totalReads = 10; // Total de lecturas a tomar
float orpValues[totalReads];  // Vector donde se almacenan las lecturas


Ticker timer;                 // Instancia de Ticker
Ticker ledTimer;             // Instancia para controlar el apagado del LED

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

  // Convertir valor crudo (0 - 1023) a voltaje
  voltage = (sensorValue / 1023.0) * 5;

  // Convertir voltaje a valor ORP (mV)
  orpValue = (voltage * calibration) * 1000;  // Según la calibración

  // Almacenar lectura
  orpValues[readCount] = orpValue;


  // Mostrar los resultados de la lectura actual
  Serial.print("Lectura ");
  Serial.print(readCount + 1);
  Serial.print(": Valor crudo del sensor ORP: ");
  Serial.println(sensorValue);
  Serial.print("Voltaje medido: ");
  Serial.print(voltage);
  Serial.println(" V");
  Serial.print("Valor ORP (mV): ");
  Serial.println(orpValue);

  // Incrementar contador de lecturas
  readCount++;

  // Si se llego a 10 lecturas
  if (readCount == totalReads) {

    // Promedio
    float sum = 0.0;
    for (int i = 0; i < totalReads; i++) {
      sum += orpValues[i];
    }
    float avgORP = sum / totalReads;

    // Mostrar promedio
    Serial.print("Promedio ORP (mV): ");
    Serial.println(avgORP);

    // Reiniciar contador y flag
    readCount = 0;
  }
}

void setup() {
  Serial.begin(115200);          // Comunicación serie
  pinMode(LED_PIN, OUTPUT);      // Pin del LED interno como salida
  pinMode(LED_PIN_VERDE, OUTPUT); // Pin del LED verde como salida
  pinMode(LED_PIN_AMARILLO, OUTPUT); // Pin del LED amarillo como salida

  // Configura el temporizador para llamar a la función cada 5 segundos
  timer.attach(5, readAnalogSignal);
}

void loop()  {}
