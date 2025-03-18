// definición de pines para el driver TB6600
#define STEP_PIN 14      // Pin de pulso (STEP)
#define DIR_PIN 12       // Pin de dirección (DIR)
#define ENABLE_PIN 13    // Pin de habilitación (ENABLE)

// configuración de velocidad
// ajuste delayStep para controlar la velocidad
unsigned int delayStep = 10000; // 10 ms entre transiciones

// número de pasos por revolución
// para un motor de 1.8°: 200 pasos para una revolución completa,
// aunque si usas microstepping el número de pasos aumentará.
const int stepsPerRev = 200;

void setup() {
  // configuración de pines como salida
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(ENABLE_PIN, OUTPUT);

  // habilitar el driver (TB6600 el ENABLE es activo en bajo)
  digitalWrite(ENABLE_PIN, LOW);

  // inicialización del estado de los pines 
  digitalWrite(STEP_PIN, LOW);
  digitalWrite(DIR_PIN, LOW);
  
  Serial.begin(115200);
  Serial.println("Control del motor paso a paso iniciado");
}

void loop() {
  // ejemplo: hacer una revolución en una dirección y luego invertir

  // establecimineto de la dirección (hacia adelante)
  digitalWrite(DIR_PIN, HIGH);
  Serial.println("Giro en dirección HIGH");
  for (int i = 0; i < stepsPerRev; i++) {
    stepMotor();
  }
  delay(1000); // Pausa de 1 segundo

  // inversión de la dirección
  digitalWrite(DIR_PIN, LOW);
  Serial.println("Giro en dirección LOW");
  for (int i = 0; i < stepsPerRev; i++) {
    stepMotor();
  }
  delay(1000); // Pausa de 1 segundo
}

// función que genera un pulso de paso
void stepMotor() {
  digitalWrite(STEP_PIN, HIGH);
  delayMicroseconds(delayStep); // pulso HIGH
  digitalWrite(STEP_PIN, LOW);
  delayMicroseconds(delayStep); // pulso LOW
}
