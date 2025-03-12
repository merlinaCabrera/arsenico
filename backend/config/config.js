// config.config.js

// Centralización de la configuración del servidor, URL y token 

module.exports = {
    server: {
      port: process.env.PORT || 3500
    },
    influx: {
      url: process.env.INFLUX_URL || "https://influxdb.gecep.ing.unlp.edu.ar", // URL de InfluxDB
      token: process.env.INFLUX_TOKEN || "Xq1qjc3zTp3zH-fv7YPE_LLW_wEoN1mzvtzsIFJPA90TeX-pASBmoVRCFBadgweaFLt_V9Dh09f86pX79ketwQ==" // token de lectura
    },
    mqtt: {
      brokerUrl: process.env.MQTT_BROKER_URL || "mqtts://mqtt-broker.com", // EJEMPLO. mqtts indica conexión segura
      options: {
        username: process.env.MQTT_USER || "usuario",
        password: process.env.MQTT_PASSWORD || "password"
      }
    }
  };
  