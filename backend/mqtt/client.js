const mqtt = require('mqtt');
const config = require('../config/config');

const client = mqtt.connect(config.mqtt.brokerUrl, config.mqtt.options);

client.on('connect', () => {
  console.log('Conectado al broker MQTT');
});

client.on('error', (err) => {
  console.error('Error en la conexión MQTT:', err);
});

module.exports = client;
