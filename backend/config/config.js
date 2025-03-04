// config.config.js

`centralización de la configuración del servidor, la URL y el token para
InfluxDB, y la información de conexión para el broker MQTT`

module.exports = {
    server: {
      port: process.env.PORT || 3000
    },
    influx: {
      url: process.env.INFLUX_URL || "https://tu-influxdb.gecep.ar", // URL de InfluxDB
      token: process.env.INFLUX_TOKEN || "tu-token-aqui"
    },
    mqtt: {
      brokerUrl: process.env.MQTT_BROKER_URL || "mqtts://tu-mqtt-broker.com", // mqtts indica conexión segura
      options: {
        username: process.env.MQTT_USER || "tu-usuario",
        password: process.env.MQTT_PASSWORD || "tu-password"
      }
    }
  };
  