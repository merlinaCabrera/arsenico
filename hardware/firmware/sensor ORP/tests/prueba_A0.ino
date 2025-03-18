const int analogInPin = A0;  // ESP8266 Analog Pin ADC0 = A0
const int numSamples = 10;  // Número de muestras para promediar (declarado como constante)
int sensorValue = 0;
float voltage = 0.0;

int sampleIndex = 0;  // Índice de la muestra actual
float sensorSum = 0;  // Suma de las lecturas para calcular el promedio

void setup() {
  Serial.begin(115200);
}

void loop() {
  // Leer la muestra del sensor
  sensorValue = analogRead(analogInPin);
  // Convertir el valor promedio a voltaje (rango de entrada 0V - 1V)
  voltage = (sensorValue / 1023.0) * 1.0;
  // Acumular el valor en la suma
  sensorSum += voltage;;


  // Incrementar el índice, reiniciando si excede el número de muestras
  sampleIndex++;
  if (sampleIndex >= numSamples) {

    // Calcular el promedio de las últimas 10 muestras
    float averageSensorValue = sensorSum / numSamples;

    // Imprimir solo el promedio de las últimas 10 muestras
    Serial.print("Promedio de las últimas 10 muestras = ");
    Serial.print(averageSensorValue, 4);
    Serial.print("\n");

    sampleIndex = 0;
    sensorSum = 0;

  }



  delay(100);
}



/*
Offset 1.65V. El rango de [-1000mV, 1000mV] del sensor deberia mapearse entre 
aproximadamente [0.65V, 2.65V]. Dado que el ESP8266 mide de 0 a 1V, el divisor
resistivo debe ajustar la señal para que los extremos del rango se ubiqen dentro del
margen de lectura.

Señal de entrada con el offset de 1.65 V ya está mapeada en el rango de 0 a 1 V por 
el divisor resistivo. La conversión a ORP entonces se realizaría centrando el valor 
de 0.5 V (aproximadamente) en 0 mV.

Verificación del divisor resistivo:  el divisor reduce la señal correctamente de 
0.65-2.65 V a 0-1 V. Si no, el rango puede estar sesgado, y los valores de conversión 
no serán precisos.

Ruido: la alta ganancia aplicada a la señal puede amplificar el ruido. Implementar 
un filtro (por ejemplo, un filtro de media móvil o de paso bajo) en la lectura para 
reducir las variaciones.


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
  

*/
