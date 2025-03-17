el backend usa un cliente oficial de InfluxDB en Node.js
se define un endpoint en el controlador que:
    - ejecuta una consulta Flux para obtener, para cada sensor ORP, los últimas 12 horas de datos (limitando a 20 puntos por sensor)
    - agrupa los resultados por el tag 'sensor_id'
    - realiza el análisis de cada sensor para determinar si alguno supera los 600mV
    - devuelve un JSON con los ddatos de cada sensor y para cada uno un flag de alerta




# mqtt
endpoint POST /api/mqtt/send-command al cual se puede enviar un body JSON que contenga un tópico y un mensaje

en el broker se centraliza la transmisión de mensajes que intermedia los clientes (backend - mcu)

# broker Mosquitto
mosquitto configurado para usar TLS (cifrado de comunicación)