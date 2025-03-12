const express = require('express');
const cors = require('cors');
const config = require('./config/config');
const influxRouter = require('./routes/influx'); // donde definiste la ruta sensor1-latest

const app = express();
app.use(cors());
app.use(express.json());

// Montar rutas
app.use('/api/influx', influxRouter);

const PORT = config.server.port || 5000;
app.listen(PORT, () => {
  console.log(`Servidor corriendo en http://localhost:${PORT}`);
});
