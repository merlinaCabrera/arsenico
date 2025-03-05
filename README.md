# arsénico
sistema embebido: monitoreo y control remoto de parámetros para el proceso de remoción de arśenico de agua en una planta de tratamiento ubicada en Pipinas.

- hardware: diseño de PCB desarrollado en etapa previa del proyecto y versiones del firmware en Arduino para ESP8266.
- backend: estructura para recuperar datos de InfluxDB mediante solicitudes GET a su API, procesaiento de datos y conexión con frontend. 
    se evalúa a futuro la conexión entre backend & MCU (vía MQTT)
- frontend: estructura visual de la aplicación web, interacción con Grafana y con backend.
- docs: documentación del proyecto 2024 - 2025.
