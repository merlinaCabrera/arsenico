import React, { useState } from "react";
import '../styles/HamburgerMenu.css'

function HamburgerMenu() {
    const [isOpen, setIsOpen] = useState(false);

    const toggleMenu = () => {
        setIsOpen(!isOpen);
    }

    return (
        <header className="hamburger-menu">
            <div className={`hamburger ${isOpen ? 'open' : '' }`} onClick={toggleMenu}>
                <span></span>
                <span></span>
                <span></span>
            </div>

            <div>
                <nav className={`menu ${isOpen ? 'active' : ''}`}>
                    <ul>
                        <li><a href="Inicio">Inicio</a></li>
                        <li><a href="Histórico">Histórico</a></li>
                    </ul>
                </nav>
            </div>
        </header>
    );
}

export default HamburgerMenu;

