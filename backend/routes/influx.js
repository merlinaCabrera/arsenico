// routes/influx.js

const express = require('express');
const router = express.Router();
const influxController = require('../controllers/influxController');

// Endpoint /api/influx/data para ejecutar getData y recuperar información de InfluxDB
router.get('/data', influxController.getData);

module.exports = router;
