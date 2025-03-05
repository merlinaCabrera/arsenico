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

# Exponer el puerto del servidor Express
EXPOSE 5000

# Ejecutar la aplicación
CMD ["npm", "start"]
