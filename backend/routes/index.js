const express = require('express');
const router = express.Router();
const influxController = require('../controllers/influxController');

// Ruta para obtener datos de ORP
router.get('/orp-data', influxController.getORPData);


module.exports = router;
