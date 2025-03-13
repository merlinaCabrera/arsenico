// config/config.js

module.exports = {
  server: {
    port: process.env.PORT || 3500
  },
  influx: {
    url: process.env.INFLUX_URL || "https://influxdb.gecep.ing.unlp.edu.ar", // URL de InfluxDB
    token: process.env.INFLUX_TOKEN || "Xq1qjc3zTp3zH-fv7YPE_LLW_wEoN1mzvtzsIFJPA90TeX-pASBmoVRCFBadgweaFLt_V9Dh09f86pX79ketwQ==", // token (asegúrate que tenga permisos de consulta/escritura)
    org: process.env.INFLUX_ORG || "arsénico"
  },
  mqtt: {
    brokerUrl: process.env.MQTT_BROKER_URL || "mqtts://mqtt-broker.com",
    options: {
      username: process.env.MQTT_USER || "usuario",
      password: process.env.MQTT_PASSWORD || "password"
    }
  }
};
