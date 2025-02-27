import React, { useState } from "react";
import { Link } from "react-router-dom";
import "../styles/HamburgerMenu.css";

function HamburgerMenu() {
  const [open, setOpen] = useState(false);

  return (
    <div>
      <div className="hamburger" onClick={() => setOpen(!open)}>
        {open ? "✖" : "☰"} {/* Cambia entre ☰ (menú) y ✖ (cerrar) */}
      </div>
      {open && (
        <div className="menu">
          <Link to="/" onClick={() => setOpen(false)}>Inicio</Link>
          <Link to="/about" onClick={() => setOpen(false)}>Acerca de</Link>
          <Link to="/contact" onClick={() => setOpen(false)}>Contacto</Link>
        </div>
      )}
    </div>
  );
}

export default HamburgerMenu;
