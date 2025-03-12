import { useEffect, useState } from "react";

// se usa fetch() para llamar al endpoint (http://localhost:3500/api/obtener-datos)

function AlertasORP() {
  const [data, setData] = useState(null);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState(null);

  useEffect(() => {
    fetch("http://localhost:3502/api/orp-data") // Ajusta el endpoint si es necesario
      .then((response) => {
        if (!response.ok) {
          throw new Error("Error en la respuesta del servidor");
        }
        return response.json();
      })
      .then((data) => {
        setData(data);
        setLoading(false);
      })
      .catch((error) => {
        setError(error.message);
        setLoading(false);
      });
  }, []);

  if (loading) return <p>Cargando datos...</p>;
  if (error) return <p>Error: {error}</p>;

  return (
    <div>
      <h2>Alertas de ORP</h2>
      {Object.entries(data.alerts).map(([sensor, alert]) => (
        <div key={sensor} style={{ color: alert ? "red" : "green" }}>
          {sensor}: {alert ? "⚠️ Alerta Activa" : "✅ Todo OK"}
        </div>
      ))}
    </div>
  );
}

export default AlertasORP;
