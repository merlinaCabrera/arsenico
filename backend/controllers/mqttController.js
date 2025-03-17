// controllers/mqttController.js
const client = require('../mqtt/client');

exports.sendCommand = (req, res) => {
  const { topic, message } = req.body;
  if (!topic || !message) {
    return res.status(400).json({ error: "Se requieren 'topic' y 'message'" });
  }

  client.publish(topic, message, { qos: 1 }, (err) => {
    if (err) {
      console.error("Error al publicar mensaje:", err);
      return res.status(500).json({ error: "Error al publicar el mensaje" });
    }
    return res.json({ message: "Mensaje publicado exitosamente" });
  });
};
