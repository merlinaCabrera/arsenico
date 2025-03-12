// src/pages/Alerts.js

import React from 'react';
import AlertORP from '../components/AlertORP';

function Alerts() {
  return (
    <div className="App">
      <header className="App-header">
        <h1 className="intro">
          Alertas de Sensores <br />
        </h1>

        <AlertORP/>
        
      </header>
    </div>
  );
}

export default Alerts;
