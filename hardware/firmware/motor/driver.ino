// Definición de pines para el driver TB6600
#define STEP_PIN 13      // Pin de pulso (PUL) STEP
#define DIR_PIN 12       // Pin de dirección (DIR)
#define ENABLE_PIN 14    // Pin de habilitación (ENA)

// Configuración de velocidad
// Ajuste delayStep para controlar la velocidad
unsigned int delayStep = 10000; // 10 ms entre transiciones

// Número de pasos por revolución 
// ESTÁNDAR 200 pasos por revolución
// Para motor de 1.8°: 200 pasos para una revolución completa (microstepping: el número de pasos aumenta)
const int stepsPerRev = 200;

void setup() {
  // Configuración de pines como salida
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(ENABLE_PIN, OUTPUT);

  // Habilitar driver (TB6600 el ENABLE activo en bajo)
  digitalWrite(ENABLE_PIN, LOW);

  // Inicialización del estado de los pines 
  digitalWrite(STEP_PIN, LOW);
  digitalWrite(DIR_PIN, LOW);
  
  Serial.begin(115200);
  Serial.println("Control del motor paso a paso iniciado");
}

void loop() {
  // Revolución en una dirección y luego invertir

  // Establecimineto de la dirección (hacia adelante)
  digitalWrite(DIR_PIN, HIGH);
  Serial.println("Giro en dirección HIGH");
  for (int i = 0; i < stepsPerRev; i++) {
    stepMotor();
  }
  delay(10000); // Pausa de 1 segundo

  // Inversión de la dirección
  digitalWrite(DIR_PIN, LOW);
  Serial.println("Giro en dirección LOW");
  for (int i = 0; i < stepsPerRev; i++) {
    stepMotor();
  }
  delay(10000); // Pausa de 1 segundo
}

// Función que genera un pulso de paso
void stepMotor() {
  digitalWrite(STEP_PIN, HIGH);
  delayMicroseconds(delayStep); // Pulso HIGH
  digitalWrite(STEP_PIN, LOW);
  delayMicroseconds(delayStep); // Pulso LOW
}