# Proyecto IoT BMP280 + ESP32 + Firebase (DSE)

Daniel Alejandro De La Cruz Hernández · Matrícula 2178079 · UANL FIME · Diseño de Sistemas Embebidos

- `firmware/`: proyecto PlatformIO (ESP32 DevKit, framework Arduino). Lee temperatura, presión y altitud con el BMP280 (I2C: SDA = GPIO 21, SCL = GPIO 22) y las envía cada 3 s a Firebase Realtime Database, nodo `/Weather`. LED de estado en GPIO 33.
  La biblioteca Firebase ESP32 Client (Mobizt, v4.1.0) se coloca manualmente en `firmware/lib/FIREBASE-ESP32`.
  Antes de compilar reemplaza `WIFI_SSID`, `WIFI_PASSWORD` y `FIREBASE_AUTH` en `src/main.cpp`.
- `web/`: aplicación web (HTML, CSS y JavaScript con Firebase JS SDK 8.10.1) publicada con Firebase Hosting en https://iot-2178079.web.app/
