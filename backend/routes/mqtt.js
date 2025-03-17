// routes/mqtt.js
const express = require('express');
const router = express.Router();
const mqttController = require('../controllers/mqttController');

// Endpoint para enviar comandos al microcontrolador
router.post('/send-command', mqttController.sendCommand);

module.exports = router;
