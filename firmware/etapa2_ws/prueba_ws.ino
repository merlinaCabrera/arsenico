#include <ESP8266WiFi.h>
#include <WebSocketsClient.h>

const char* ssid = "Fibertel WiFi565 2.4GHz";
const char* password = "0142785516";
WebSocketsClient webSocket;

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
    switch(type) {
        case WStype_TEXT:
            Serial.printf("Mensaje recibido: %s\n", payload);
            if (strcmp((char*)payload, "ABRIR_VALVULA") == 0) {
                // Código para abrir la válvula
                Serial.println("Abriendo válvula...");
            }
            break;
    }
}

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nConectado a WiFi");

    webSocket.begin("192.168.0.59", 3500, "/"); 
    webSocket.onEvent(webSocketEvent);
}

void loop() {
    webSocket.loop();
}
