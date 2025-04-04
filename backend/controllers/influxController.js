// controllers/influxController.js

const { InfluxDB } = require('@influxdata/influxdb-client');
const config = require('../config/config');

const org = config.influx.org;
const influxDB = new InfluxDB({ url: config.influx.url, token: config.influx.token });
const queryApi = influxDB.getQueryApi(org);

exports.getORPData = async (req, res) => {
  const fluxQuery = `
    from(bucket: "ORP sensor simulation")
      |> range(start: -1)
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
      // Filtrar los puntos que superen el umbral (por ejemplo, 650 mV)
      const alerts = sensorData.filter(data => data.orp_value > 695);
      sensorAlerts[sensorId] = alerts;
      
   if (alerts.length > 0 && global.wsBroadcast) {
  const alertMessage = {
    sensor: sensorId,
    alerts: alerts
  };
  global.wsBroadcast(JSON.stringify(alertMessage));
  console.log(`Alerta enviada via WebSocket para sensor ${sensorId}`);
}


    }

    res.json({ data: resultsBySensor, alerts: sensorAlerts });
  } catch (error) {
    console.error('Error en getORPData:', error);
    res.status(500).json({ error: "No se pudieron obtener los datos de InfluxDB." });
  }
};
