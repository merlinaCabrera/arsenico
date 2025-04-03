const express = require('express');
const cors = require('cors');
const config = require('./config/config');
const influxRouter = require('./routes/influx');
const testRouter = require('./routes/test');
const WebSocket = require('ws');

const app = express();
app.use(cors());
app.use(express.json());

const server = require('http').createServer(app);
const wss = new WebSocket.Server({ server });

// Función para emitir mensajes a todos los clientes conectados
global.wsBroadcast = (message) => {
  wss.clients.forEach(client => {
    if (client.readyState === WebSocket.OPEN) {
      client.send(message);
    }
  });
};

wss.on('connection', (ws) => {
  console.log('Cliente conectado a WebSocket');
  ws.on('message', (message) => {
    console.log(`Mensaje recibido: ${message}`);
  });
  ws.on('close', () => {
    console.log('Cliente desconectado');
  });
});

// Monta las rutas HTTP normales
app.use('/api/influx', influxRouter);
app.use('/api/test', testRouter);

app.get('/', (req, res) => {
  res.send("Servidor WebSocket funcionando");
});

const PORT = config.server.port || 3500;
server.listen(PORT, () => {
  console.log(`Servidor corriendo en http://localhost:${PORT}`);
});
