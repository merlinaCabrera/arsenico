const axios = require('axios');
const config = require('../config/config');

/**
 * Obtiene el último valor de sensorORP_1
 * y determina si hay alerta si orp_value > 600
 */
exports.getSensor1Latest = async (req, res) => {
  try {
    // Consulta Flux para obtener el último valor de sensorORP_1
    const query = `
      from(bucket: "ORP sensor simulation")
  |> range(start: -12h)
  |> filter(fn: (r) => r._measurement == "promedio_movil_ORP")
  |> filter(fn: (r) => r._field == "orp_value")
  |> group(columns: ["sensor_id"])
  |> last()


    `;

    const response = await axios.post(
      `${config.influx.url}/api/v2/query`,
      { query },
      {
        headers: {
          'Authorization': `Token ${config.influx.token}`,
          'Content-Type': 'application/vnd.flux'
        }
      }
    );

    // `response.data` es un CSV en formato Flux
    const csv = response.data;
    // Inicializamos valor
    let orpValue = null;

    // Parseamos línea a línea
    const lines = csv.split('\n');
    for (const line of lines) {
      // Ignoramos líneas vacías o comentarios que empiezan con '#'
      if (!line.trim() || line.startsWith('#')) continue;

      // Normalmente, en Flux CSV, la columna _value está en la posición 4
      // (dependiendo de la versión y la consulta, podría variar)
      const cols = line.split(',');
      // Un ejemplo de línea (sin #) podría ser:
      //  "0,2023-01-01T12:00:00Z,2023-01-01T13:00:00Z,2023-01-01T12:30:00Z,567.89,...."
      //  donde cols[4] = "567.89"
      const valueIndex = 4; 
      orpValue = parseFloat(cols[valueIndex]);
    }

    if (orpValue === null) {
      // Significa que no se encontró dato
      return res.json({ orpValue: null, alert: false, message: "No se encontró un valor reciente para sensorORP_1." });
    }

    // Verificar si hay alerta (por ejemplo, si supera 600 mV)
    const alert = orpValue > 600;

    // Devolvemos la respuesta al frontend
    res.json({ orpValue, alert });
  } catch (error) {
    console.error("Error al obtener datos de InfluxDB:", error.response ? error.response.data : error.message);
    res.status(500).json({ error: "No se pudieron obtener los datos." });
  }
  
};
