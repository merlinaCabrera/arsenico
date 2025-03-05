# Imagen base de Node
FROM node:16

# Establecer el directorio de trabajo
WORKDIR /app

# Copiar los archivos package.json y package-lock.json
COPY package*.json ./

# Instalar dependencias
RUN npm install

# Copiar el resto del código
COPY . .

# Construir la aplicación
RUN npm run build

# Exponer el puerto en el que la app va a estar disponible
EXPOSE 3000

# Ejecutar la aplicación
CMD ["npm", "start"]
