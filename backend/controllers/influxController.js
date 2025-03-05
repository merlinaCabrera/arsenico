// controllers/influxController.js

// axios para solicitud GET a la API REST de InfluxDB
const axios = require('axios');
const config = require('../config/config');

// Obtener datos de InfluxDB
exports.getData = async (req, res) => {
  try {

    // Seleccionar últimas 50 mediciones de ORP
    const query = 'SELECT * FROM "promedio_movil_ORP" ORDER BY time DESC LIMIT 50';
    
    // Solicitud GET 
    const response = await axios.get(`${config.influx.url}/query`, {
      params: { q: query, db: condig.influx.bucket },
      headers: {
        'Authorization': `Token ${config.influx.token}`
      }
    });

    // Procesamiento de respuesta
    const series = response.data.results[0].series;
    if (!series) {
      return res.json({ data: [] });
    }

    const values = series[0].values;  // Array [time, orp_value]

    // Procesar cada registro. Marcar si ORP > 4
    const processed = values.map(point => {
      const [time, orp_value] = point;
      const alert = orp_value > 4;
      return { time, orp_value, alert };
    });

    res.json({ data: processed });
  } catch (error) {
    console.error('Error al consultar datos históricos:', error);
    res.status(500).json({ error: 'Error al consultar InfluxDB' });
  }

  
};
