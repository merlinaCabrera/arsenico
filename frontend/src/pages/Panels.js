// src/pages/Panels.js
import React from 'react';

function Panels() {
  return (
    <div className="App">
      <header className="App-header">
        <h1 className="intro">
          Paneles de Análisis de Datos <br />
        </h1>
        <p className='parrafo'>
          En los siguientes paneles se pueden observar los datos de ORP captados por los distintos sensores. 
    
          En este caso los sensados fueron simulados (random entre 300 y 700mV).
        </p>

        <br/>
        <h1 className="intro">
          Sensor ORP - ID = 1
        </h1>
        <iframe
          src="http://localhost:4000/d-solo/cefjvarogfd34e/new-dashboard?orgId=1&from=1741718458000&to=1741719058000&panelId=1"
          width="700"
          height="300"
          frameBorder="1"
          title="Dashboard Panel 1"
        ></iframe>
        <br/>
        <br/>
        <h1 className="intro">
          Sensor ORP - ID = 2
        </h1>
        <iframe
          src="http://localhost:4000/d-solo/cefjvarogfd34e/new-dashboard?orgId=1&from=1741719298000&to=1741719898000&panelId=2"
          width="700"
          height="300"
          frameBorder="1" 
          title="Dashboard Panel 2"
        ></iframe>
        <br/>
        <br/>
        <h1 className="intro">
          Sensor ORP - ID = 3
        </h1>
        <iframe
          src="http://localhost:4000/d-solo/cefjvarogfd34e/new-dashboard?orgId=1&from=1741720558000&to=1741721158000&panelId=3"
          width="700"
          height="300"
          frameBorder="1"
          title="Dashboard Panel 3"
        ></iframe>
      </header>
    </div>
  );
}

export default Panels;
