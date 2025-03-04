// controllers/influxController.js

// utiliza axios para hacer una solicitud GET a la API REST de InfluxDB
// la consulta se envía como parámetro y se usa el token para autenticarción
// respuesta en formato JSON

const axios = require('axios');
const config = require('../config/config');

// obtener datos de InfluxDB
exports.getData = async (req, res) => {
  try {
    // consulta a InfluxDB
    const query = 'SELECT * FROM "mediciones" LIMIT 10';
    const response = await axios.get(`${config.influx.url}/query`, {
      params: { q: query },
      headers: {
        'Authorization': `Token ${config.influx.token}`
      }
    });
    res.json(response.data);
  } catch (error) {
    console.error('Error al obtener datos de InfluxDB:', error);
    res.status(500).json({ error: 'Error al consultar InfluxDB' });
  }
};
