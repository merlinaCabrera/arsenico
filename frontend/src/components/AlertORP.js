import React, { useState, useEffect } from 'react';
import axios from 'axios';

const AlertORP = () => {
  const [alertas, setAlertas] = useState([]);

  const fetchData = async () => {
    try {

      // Se hace la solicitud GET a la API REST del backend
      const response = await axios.get('http://localhost:3000/api/influx/historical');
      
      // Respuesta en formato JSON
      const datos = response.data.data;

      // Filtrar los datos donde la alerta está activa (orp_value > 4)
      const alerts = datos.filter(item => item.alert === true);
      setAlertas(alerts);
    } catch (error) {
      console.error("Error al obtener alertas:", error);
    }
  };

  useEffect(() => {
    fetchData();

    // Actualizar continuamente
    const interval = setInterval(fetchData, 10000); // Actualiza cada 10 segundos
    return () => clearInterval(interval);
  }, []);

  return (
    <div>
      {alertas.length === 0 ? (
        <p>No hay alertas</p>
      ) : (
        <ul>
          {alertas.map((alerta, index) => (
            <li key={index}>
              Alerta: Valor ORP {alerta.orp_value} registrado a las {alerta.time}
            </li>
          ))}
        </ul>
      )}
    </div>
  );
};

export default AlertORP;
