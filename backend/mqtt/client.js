// mqtt/client.js

// establecer la conexión con el broker MQTT utilizando config/config.js
// el cliente se conecta de forma segura y está listo para enviar o recibir msj

const mqtt = require('mqtt');
const config = require('../config/config');

const client  = mqtt.connect(config.mqtt.brokerUrl, config.mqtt.options);

client.on('connect', () => {
  console.log('Conectado al broker MQTT');
  // suscripción a tópicos, por ejemplo:
  // client.subscribe('comandos/mcu', (err) => { ... });
});

client.on('error', (err) => {
  console.error('Error en la conexión MQTT:', err);
});

module.exports = client;
