// app.js
const express = require('express');
const cors = require('cors');
const config = require('./config/config');
const influxRouter = require('./routes/influx');
const testRouter = require('./routes/test');  // Importa el router de test

const app = express();
app.use(cors());
app.use(express.json());

// Monta las rutas
app.use('/api/influx', influxRouter);
app.use('/api/test', testRouter);  // Monta la ruta de test

const PORT = config.server.port || 5000;
app.listen(PORT, () => {
  console.log(`Servidor corriendo en http://localhost:${PORT}`);
});
