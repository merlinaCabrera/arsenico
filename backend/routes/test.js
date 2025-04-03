const express = require('express');
const router = express.Router();

router.get('/test-alert', (req, res) => {
  // Simula una alerta de prueba
  const alertMessage = {
    sensor: 'test_sensor',
    alerts: [
      {
        time: new Date().toISOString(),
        orp_value: 700
      }
    ]
  };

  // Emite el mensaje vía WebSocket usando la función global definida en app.js
  if (global.wsBroadcast) {
    global.wsBroadcast(JSON.stringify(alertMessage));
    console.log("Alerta enviada vía WebSocket:", alertMessage);
    res.send("Alerta publicada");
  } else {
    res.status(500).send("WebSocket no disponible");
  }
});

module.exports = router;
