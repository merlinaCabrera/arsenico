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
            <Routes>
                <Route path="/" element={<Home />} />
            </Routes>
                    
          <h1 className='titulo'> Planta de Tratamiento de Arsénico </h1>
          <Button />



        </header>
    </div>
  );
}

export default App;
