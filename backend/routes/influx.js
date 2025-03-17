const express = require('express');
const router = express.Router();
const influxController = require('../controllers/influxController');

// Define la ruta para obtener datos ORP y alertas
router.get('/orp-data', influxController.getORPData);

module.exports = router;
