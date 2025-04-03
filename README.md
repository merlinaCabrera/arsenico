# 🏭 Monitoreo y Control Remoto en Planta de Tratamiento  

## 📌 Descripción del Proyecto  
Este sistema embebido permite el monitoreo y control remoto de los parámetros involucrados en el proceso de remoción de arsénico en una planta de tratamiento de agua ubicada en **Pipinas**.  

### 🛠️ Componentes del Proyecto  

- **🔌 Firmware:**: Firmware basado en **Arduino para ESP8266**, modularizado en distintas etapas.
- **📡 Backend:** Recuperación de datos desde **InfluxDB** mediante solicitudes `GET` a su API, procesamiento de información y conexión con el frontend. 
Integración del backend con el MCU mediante **Web Sockets**.  
- **🖥️ Frontend:** Interfaz visual de la aplicación web, interacción con **Grafana** y comunicación con el backend.  
- **📄 Documentación:** Registro detallado del desarrollo del proyecto (2024 - 2025).  

---

# 🚀 Despliegue de la Aplicación Web con Docker  

## 📋 Requisitos Previos  
Asegúrate de tener instalados los siguientes componentes en tu sistema:  

- 🐳 **Docker** → [Guía de instalación](https://docs.docker.com/get-docker/)  
- 🛠️ **Docker Compose** → [Guía de instalación](https://docs.docker.com/compose/install/)  

## 🏗️ Pasos para el Despliegue Local  
```bash
# 1️⃣ Clonar el repositorio  
git clone https://github.com/merlinaCabrera/arsenico.git
cd arsenico

# 2️⃣ Construir y levantar los contenedores
docker-compose up --build

# 3️⃣ Acceder a la aplicación
# Una vez en ejecución, abre tu navegador y accede a:
http://localhost:3000

# 4️⃣ Detener la aplicación
# Para apagar los contenedores, ejecuta:
docker-compose down


# ⚠️ Solución a Problemas Comunes
# Si los cambios recientes no se reflejan en la aplicación, intenta reconstruir sin caché:
docker-compose build --no-cache
docker-compose up
