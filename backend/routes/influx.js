const express = require('express');
const router = express.Router();
const influxController = require('../controllers/influxController');

// Ruta para obtener el último valor de sensorORP_1
router.get('/sensor1-latest', influxController.getSensor1Latest);

module.exports = router;
