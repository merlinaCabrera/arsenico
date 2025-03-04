// routes/influx.js

// definición de endpoint /api/influx/data para ejecutar getData y recuperar información de InfluxDB

const express = require('express');
const router = express.Router();
const influxController = require('../controllers/influxController');

// definición de la ruta para obtener datos de InfluxDB
router.get('/data', influxController.getData);

module.exports = router;
