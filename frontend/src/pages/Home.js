// src/pages/Home.js
import React from 'react';
import imagen1 from '../media/imagen1.jpeg';
import imagen2 from '../media/imagen2.jpeg';
import imagen3 from '../media/imagen3.jpeg';
import imagen4 from '../media/imagen4.jpeg';
import imagen5 from '../media/imagen5.jpeg';
import imagen6 from '../media/imagen6.jpeg';

import '../styles/Home.css';

function Home() {
    return (
        <div className="home-container">
        <h1 className="intro">
          Bienvenido! <br />
        </h1>
          <div className="photo-grid">
            <img src={imagen2} alt="Foto 1" />
            <img src={imagen1} alt="Foto 2" />
            <img src={imagen3} alt="Foto 3" />
            <img src={imagen4} alt="Foto 4" />
            <img src={imagen5} alt="Foto 5" />
            <img src={imagen6} alt="Foto 6" />

          </div>

          <div className="text-section">
        <h1 className="intro">Problemática</h1>
        <p className="parrafo">
          La llanura Chaco-Pampeana es una región conocida a nivel mundial por el alto contenido
          de arsénico (As) en aguas subterráneas, metaloide de elevada toxicidad cuya ingesta
          crónica a través del agua de consumo produce graves efectos a la salud. Desde nuestra
          Facultad y el CONICET, se construyeron prototipos de plantas de tratamiento para escalas
          de hasta 15000 litros/día probados exitosamente en localidades de la Provincia de Buenos
          Aires. Para garantizar la eficacia, el control de riesgos y la sustentabilidad del proyecto, es
          necesario que las plantas desarrolladas sean en primer lugar optimizadas (adecuadas) con
          asistencia del personal de la UNLP de acuerdo con las características particulares del
          agua subterránea a tratar. El fuerte vínculo establecido con los cooperativistas de Pipinas
          y el trabajo en conjunto han permitido grandes avances en la optimización de la planta y su
          puesta en servicio para esta comunidad.
          <br/>
          La planta consta de varias etapas, algunas de las cuales requieren que el valor de pH se
          mantenga en un rango determinado para la adecuada y eficiente ocurrencia de los
          procesos fisicoquímicos involucrados, y adicionalmente, se requiere conocer el “estado”
          del agua (a través de otras variables de interés) en distintas etapas y al final del proceso.
          <br/>
          Se plantean como objetivos de la PPS, la participación en las actividades técnicas
          referidas al desarrollo de sensores y sistemas de control, así como en otras actividades
          relacionadas a la puesta en servicio de la planta en la comunidad de Pipinas en el marco
          del proyecto de extensión mencionado.
          <br/> 
          Se espera que así pueda aprovechar esta experiencia, no solamente por los
          conocimientos técnicos que podrá incorporar, sino también para familiarizarse con la
          problemática del arsénico en pequeñas comunidades y la búsqueda de soluciones en
          cooperación con la cooperativa de Pipinas y nuestro grupo de trabajo, que puedan ser
        apropiadas por la comunidad y sostenidas en el tiempo.
        </p>
      </div>

        </div>

        
    );
}

export default Home;
