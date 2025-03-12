// routes/influx.js

const express = require('express');
const router = express.Router();
const influxController = require('../controllers/influxController');

// Ruta para obtener datos ORP y alertas de cada sensor
router.get('/orp-data', influxController.getORPData);

module.exports = router;
