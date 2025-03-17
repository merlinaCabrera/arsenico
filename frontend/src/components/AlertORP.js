import { useEffect, useState } from "react";

function AlertasORP() {
  const [data, setData] = useState(null);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState(null);

  useEffect(() => {
    fetch("http://localhost:3500/api/influx/orp-data")
      .then((response) => {
        if (!response.ok) {
          throw new Error("Error en la respuesta del servidor");
        }
        return response.json();
      })
      .then((data) => {
        console.log("Data recibida:", data);
        setData(data);
        setLoading(false);
      })
      .catch((error) => {
        console.error("Error en fetch:", error);
        setError(error.message);
        setLoading(false);
      });
  }, []);

  if (loading) return <p>Cargando datos...</p>;

  // Manejo de error con mensaje más claro y botón de reintentar
  if (error) return (
    <div style={{ color: "red", textAlign: "center" }}>
      <p>❌ Error al cargar datos: {error}</p>
      <button onClick={() => window.location.reload()} style={{ padding: "8px", cursor: "pointer" }}>
        Reintentar
      </button>
    </div>
  );

  return (
    <div>
      {data.alerts && Object.entries(data.alerts).map(([sensor, alertData]) => (
        <div key={sensor} style={{ marginBottom: "20px" }}>
          <h3>Alertas {sensor}</h3>
          {alertData.length > 0 ? (
            <ul>
              {alertData.map((item, index) => (
                <li key={index}>
                  ⚠️ ORP: {item.orp_value} mV - Tomado el: {new Date(item.time).toLocaleString()}
                </li>
              ))}
            </ul>
          ) : (
            <p style={{ color: "green" }}>No hay alertas</p>
          )}
        </div>
      ))}
    </div>
  );
}

export default AlertasORP;
