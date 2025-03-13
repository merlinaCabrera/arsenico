// controllers/influxController.js

const { InfluxDB } = require('@influxdata/influxdb-client');
const config = require('../config/config');

const org = config.influx.org;
const influxDB = new InfluxDB({ url: config.influx.url, token: config.influx.token });
const queryApi = influxDB.getQueryApi(org);

exports.getORPData = async (req, res) => {
  // Flux query: obtenemos datos, agrupados por sensor_id, limitando a 20 puntos por sensor
  const fluxQuery = `
    from(bucket: "ORP sensor simulation")
      |> range(start: -100h)
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
          // Inicializamos el arreglo para el sensor si aún no existe
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

    // Para cada sensor, recopilamos los datos que generan alerta (orp_value > 600)
    const sensorAlerts = {};
    for (const sensorId in resultsBySensor) {
      const sensorData = resultsBySensor[sensorId];
      // Filtramos los puntos que superen 600 mV
      sensorAlerts[sensorId] = sensorData.filter(data => data.orp_value > 695);
    }

    // Retornamos la respuesta con los datos y las alertas por sensor
    res.json({ data: resultsBySensor, alerts: sensorAlerts });
  } catch (error) {
    console.error('Error en getORPData:', error);
    res.status(500).json({ error: "No se pudieron obtener los datos de InfluxDB." });
  }
};
