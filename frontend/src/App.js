// src/App.js
import './App.css';
import { Routes, Route } from 'react-router-dom';
import HamburgerMenu from './components/HamburgerMenu';
import Home from './pages/Home';
import Panels from './pages/Panels';
import Alerts from './pages/Alerts';

function App() {
  return (
    <div className="App">
      <header className="App-header">
        <HamburgerMenu />
        <h1 className="titulo">Planta de Tratamiento de Arsénico</h1>
        <Routes>
          <Route path="/" element={<Home />} />
          <Route path="/panels" element={<Panels />} />
          <Route path="/alerts" element={<Alerts />} />
        </Routes>
      </header>
    </div>
  );
}

export default App;
