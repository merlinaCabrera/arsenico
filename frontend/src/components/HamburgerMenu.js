// src/components/HamburgerMenu.js
import React, { useState } from "react";
import { Link } from "react-router-dom";
import '../styles/HamburgerMenu.css';

function HamburgerMenu() {
  const [isOpen, setIsOpen] = useState(false);

  const toggleMenu = () => {
    setIsOpen(!isOpen);
  };

  return (
    <header className="hamburger-menu">
      <div className={`hamburger ${isOpen ? 'open' : ''}`} onClick={toggleMenu}>
        <span></span>
        <span></span>
        <span></span>
      </div>
      <nav className={`menu ${isOpen ? 'active' : ''}`}>
        <ul>
          <li><Link to="/">Inicio</Link></li>
          <li><Link to="/panels">Paneles</Link></li>
          <li><Link to="/alerts">Alertas</Link></li>
        </ul>
      </nav>
    </header>
  );
}

export default HamburgerMenu;
