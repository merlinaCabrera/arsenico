# Control de Motor Paso a Paso
El motor es controlado con el driver TB6600, lo fundamental es generar pulsos (señal PUL) y definir la dirección (señal DIR). 
el TB6600 interpreta cada pulso como un paso (microstepping configurado). Los pasos estándar son 200.
Y la señal DIR indica la dirección de giro. También es importante controlar la señal de habilitación (ENA).

* Señal ENABLE: Si ENA+ y ENA- permanencen desconectados se mantiene siempre habilitado.



delayMicroseconds() para ajuste de volecidad y aceleración.
Biblioteca AccelStepper permite control más fino de velocidad y posicionamiento.

Microstepping de 1/8: revolución completa requiere 200*8=1600 pasos.
Variable: stepsPerRev.

