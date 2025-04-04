// controllers/influxController.js
const { InfluxDB } = require('@influxdata/influxdb-client');
const config = require('../config/config');

const org = config.influx.org;
const influxDB = new InfluxDB({ url: config.influx.url, token: config.influx.token });
const queryApi = influxDB.getQueryApi(org);

// Objeto global para almacenar la última marca de tiempo enviada por sensor
const lastAlertTimestamp = {};

// Función para convertir una marca de tiempo ISO a milisegundos
function timestampMs(isoString) {
  return new Date(isoString).getTime();
}

exports.getORPData = async (req, res) => {
  const fluxQuery = `
    from(bucket: "ORP sensor simulation")
      |> range(start: -1h)
      |> filter(fn: (r) => r._measurement == "promedio_movil_ORP")
      |> filter(fn: (r) => r._field == "orp_value")
      |> group(columns: ["sensor_id"])
      |> sort(columns: ["_time"], desc: true)
      |> limit(n: 20)
  `;

  let resultsBySensor = {};

  try {
    await new Promise((resolve, reject) => {
      queryApi.queryRows(fluxQuery, {
        next: (row, tableMeta) => {
          const o = tableMeta.toObject(row);
          const sensorId = o.sensor_id;
          if (!resultsBySensor[sensorId]) {
            resultsBySensor[sensorId] = [];
          }
          resultsBySensor[sensorId].push({
            time: o._time,
            orp_value: parseFloat(o._value)
          });
        },
        error: (error) => {
          console.error('Error al consultar InfluxDB:', error);
          reject(error);
        },
        complete: () => {
          resolve();
        },
      });
    });

    const sensorAlerts = {};
    for (const sensorId in resultsBySensor) {
      const sensorData = resultsBySensor[sensorId];
      // Filtrar los puntos que superen el umbral (por ejemplo, 600 mV)
      let newAlerts = sensorData.filter(data => data.orp_value > 600);
      
      // Si ya se enviaron alertas previamente para este sensor, filtra las que sean nuevas
      if (lastAlertTimestamp[sensorId]) {
        newAlerts = newAlerts.filter(data => timestampMs(data.time) > lastAlertTimestamp[sensorId]);
      }
      
      if (newAlerts.length > 0) {
        // Actualiza el último timestamp enviado para este sensor
        // Suponemos que los datos están ordenados descendente; usamos el mayor (más reciente)
        const latestTime = Math.max(...newAlerts.map(data => timestampMs(data.time)));
        lastAlertTimestamp[sensorId] = latestTime;
        
        sensorAlerts[sensorId] = newAlerts;
        
        if (global.wsBroadcast) {
          const alertMessage = {
            sensor: sensorId,
            alerts: newAlerts
          };
          global.wsBroadcast(JSON.stringify(alertMessage));
          console.log(`Alerta enviada via WebSocket para sensor ${sensorId}:`, alertMessage);
        }
      }
    }

    if (res && res.json) {
      res.json({ data: resultsBySensor, alerts: sensorAlerts });
    }
  } catch (error) {
    console.error('Error en getORPData:', error);
    if (res && res.status) {
      res.status(500).json({ error: "No se pudieron obtener los datos de InfluxDB." });
    }
  }
};
