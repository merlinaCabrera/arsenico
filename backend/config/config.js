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
    brokerUrl: "mqtt://localhost:1883",  // Asegúrate de que no diga "mqtts" ni tenga otro puerto
    options: {
      username: "usuario",   // Si no usas autenticación, elimina estas líneas
      password: "Olivia1602"
    }
  }

};
