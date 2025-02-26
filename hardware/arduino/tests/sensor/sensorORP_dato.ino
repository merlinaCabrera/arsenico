const int analogInPin = A0;  // ESP8266 Analog Pin ADC0 = A0
const int numSamples = 10;  // Número de muestras para promediar (declarado como constante)
int sensorValue = 0;
float voltage = 0.0;
float orpValue = 0.0;

int sampleIndex = 0;  // Índice de la muestra actual
float sensorSum = 0;  // Suma de las lecturas para calcular el promedio

void setup() {
  Serial.begin(115200);
}

void loop() {
  sensorValue = analogRead(analogInPin);
  Serial.print("sensor = ");
  Serial.print(sensorValue);
  Serial.print("\n");

  // Convertir valor a voltaje (rango de entrada 0V - 1V)
  voltage = (sensorValue / 1023.0) * 1.0;
  Serial.print("rango 0V,1V = ");
  Serial.print(voltage, 4);
  Serial.print("\n");


  // Convertir voltaje a rango de ORP (mV) ([0V, 1V] -> [-1000mV, 1000mV]])
  orpValue = ((voltage - 0.65) *1000* 0.100/0.270); 
  Serial.print("rango -1000mV,1000mV = ");
  Serial.print(orpValue);
  Serial.print("\n\n\n"); 
}


