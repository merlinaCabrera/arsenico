// controllers/influxController.js

const { InfluxDB } = require('@influxdata/influxdb-client');
const config = require('../config/config');
const mqttClient = require('../mqtt/client'); // Asegúrate de requerir el cliente MQTT

const org = config.influx.org;
const influxDB = new InfluxDB({ url: config.influx.url, token: config.influx.token });
const queryApi = influxDB.getQueryApi(org);

exports.getORPData = async (req, res) => {
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
      // Filtrar los puntos que superen el umbral (por ejemplo, 695 mV)
      const alerts = sensorData.filter(data => data.orp_value > 695);
      sensorAlerts[sensorId] = alerts;
      
      // Si hay alertas para el sensor, publica un mensaje MQTT
      if (alerts.length > 0) {
        const alertMessage = {
          sensor: sensorId,
          alerts: alerts
        };
        mqttClient.publish('alertas/orp', JSON.stringify(alertMessage), { qos: 1 }, (err) => {
          if (err) {
            console.error("Error enviando alerta MQTT:", err);
          } else {
            console.log(`Alerta publicada para sensor ${sensorId}`);
          }
        });
      }
    }

    res.json({ data: resultsBySensor, alerts: sensorAlerts });
  } catch (error) {
    console.error('Error en getORPData:', error);
    res.status(500).json({ error: "No se pudieron obtener los datos de InfluxDB." });
  }
};
