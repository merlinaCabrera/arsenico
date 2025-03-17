// routes/test.js
const express = require('express');
const router = express.Router();
const mqttClient = require('../mqtt/client');

router.get('/test-alert', (req, res) => {
  if (!mqttClient.connected) {
    console.error("MQTT client not connected");
    return res.status(500).send("MQTT client not connected");
  }
  
  const alertMessage = {
    sensor: 'test_sensor',
    alerts: [
      {
        time: new Date().toISOString(),
        orp_value: 700  // Valor para simular una alerta
      }
    ]
  };

  mqttClient.publish('alertas/orp', JSON.stringify(alertMessage), { qos: 1 }, (err) => {
    if (err) {
      console.error("Error publicando alerta:", err);
      return res.status(500).send("Error publicando alerta");
    }
    console.log("Alerta publicada desde ruta de prueba");
    return res.send("Alerta publicada");
  });
});

module.exports = router;
