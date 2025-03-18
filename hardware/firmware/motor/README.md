# control de motor paso a paso
el motor es controlado con el driver TB6600, lo fundamental es generar pulsos (señal STEP) y definir la dirección (señal DIR). 
el TB6600 interpreta cada pulso como un paso (microstepping configurado) y la señal DIR indica la dirección de giro. también es importante controlar la señal de habilitaciṕn (ENABLE)



delayMiceoseconds() para ajuste de volecidad y aceleración
biblioteca AccelStepper permite control más fino de velocidad y posicionamiento

microstepping de 1/8: revolución completa requiere 200*8 pasos
variable: stersPerVer

