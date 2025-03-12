// app.js

const express = require('express');
const cors = require('cors');
const config = require('./config/config');
const influxRouter = require('./routes/influx');

const app = express();
app.use(cors());
app.use(express.json());

// Montamos las rutas para InfluxDB
app.use('/api/influx', influxRouter);

const PORT = config.server.port || 5000;
app.listen(PORT, () => {
  console.log(`Servidor corriendo en http://localhost:${PORT}`);
});

app.get("/api/orp-data", (req, res) => {
  res.json({
    alerts: {
      sensor1: true,
      sensor2: false,
    },
  });
});
