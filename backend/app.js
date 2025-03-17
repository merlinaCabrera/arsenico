// app.js
const express = require('express');
const cors = require('cors');
const config = require('./config/config');
const influxRouter = require('./routes/influx');
const mqttRouter = require('./routes/mqtt'); // <-- Importar la ruta de MQTT

const app = express();
app.use(cors());
app.use(express.json());

// Montamos las rutas
app.use('/api/influx', influxRouter);
app.use('/api/mqtt', mqttRouter); // <-- Montar la ruta de MQTT

const PORT = config.server.port || 5000;
app.listen(PORT, () => {
  console.log(`Servidor corriendo en http://localhost:${PORT}`);
});
