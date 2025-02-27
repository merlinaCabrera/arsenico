import './App.css';
import { BrowserRouter as Router, Routes, Route } from "react-router-dom"
import { Button } from './components/Button';
import HamburgerMenu from './components/HamburgerMenu';
import Home from './pages/Home';

function App() {
  return (
    <div className='App'>
      
        <header className='App-header'>
          <HamburgerMenu />
          <h1 className='titulo'> Planta de Tratamiento de Arsénico </h1>
          <h1 className='intro'> 
            Paneles de análisis de datos <br/>  
          </h1>
            <p> En el siguiente panel se pueden observar los datos de ORP 
              captados por el sensor de ORP del agua purificada.</p>
          <iframe
            src="http://localhost:4000/d-solo/ae3u0cqlq9hq8c/valor-crudo-orp?orgId=1&from=1731511421771&to=1731512321771&refresh=10s&panelId=1"
            width="450"
            height="200"
            frameBorder="0"
          ></iframe>
          
        </header>
    </div>
  );
}

export default App;
