#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_BMP280.h>
#include <WiFi.h>
#include <FirebaseESP32.h>

// DATOS DE WIFI (reemplazar por los propios; no subir datos reales a GitHub)
#define WIFI_SSID "TU_RED_WIFI"
#define WIFI_PASSWORD "TU_CONTRASENA"
// DATOS DE FIREBASE
#define FIREBASE_HOST "iot-2178079-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "TU_CLAVE_DE_LA_BASE_DE_DATOS"
// OBJETOS FIREBASE
FirebaseData fbdo;
FirebaseJson updateSensorValue;
// SENSOR BMP280
Adafruit_BMP280 bmp;
// VARIABLES DE LOS SENSORES
float Temperatura = 0;
float Presion = 0;
float Altitud = 0;

// LED VERDE DE ESTADO
#define LED_VERDE 33
bool estatus = false;

// PROTOTIPO DE FUNCIÓN
void UpdateFirebase();

// SETUP
void setup()
{
  Serial.begin(9600);
  delay(1000);

  Wire.begin(21, 22);
  pinMode(LED_VERDE, OUTPUT);
  digitalWrite(LED_VERDE, LOW);

  // INICIAR BMP280
  Serial.println();
  Serial.println("BMP280 Forced Mode Test.");

  if (!bmp.begin())
  {
    Serial.println("No se encontro el sensor BMP280.");
    while (1)
    {
      delay(10);
    }
  }
  Serial.println("BMP280 encontrado correctamente.");

  bmp.setSampling(
    Adafruit_BMP280::MODE_FORCED,
    Adafruit_BMP280::SAMPLING_X2,
    Adafruit_BMP280::SAMPLING_X16,
    Adafruit_BMP280::FILTER_X16,
    Adafruit_BMP280::STANDBY_MS_500
  );

  // CONEXIÓN WIFI
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Conectando a WiFi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.println("WiFi conectado correctamente.");
  Serial.print("Direccion IP: ");
  Serial.println(WiFi.localIP());

  // CONEXIÓN FIREBASE
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
  Serial.println("Firebase iniciado.");
  Serial.println();
}

void loop()
{
  if (bmp.takeForcedMeasurement())
  {
    Temperatura = bmp.readTemperature();
    Serial.print("Temperatura = ");
    Serial.print(Temperatura);
    Serial.println(" °C");

    Presion = bmp.readPressure() * 0.00750062;
    Serial.print("Presion = ");
    Serial.print(Presion);
    Serial.println(" mmHg");

    Altitud = bmp.readAltitude(1013.25);
    Serial.print("Altitud aproximada = ");
    Serial.print(Altitud);
    Serial.println(" m");

    Serial.println();
    UpdateFirebase();
  }
  else
  {
    Serial.println("Error al realizar la medicion.");
  }
  delay(3000);
}

void UpdateFirebase()
{
  updateSensorValue.clear();
  updateSensorValue.add("Temperatura", Temperatura);
  updateSensorValue.add("Presion", Presion);
  updateSensorValue.add("Altitud", Altitud);

  if (Firebase.RTDB.updateNode(&fbdo, "/Weather", &updateSensorValue))
  {
    Serial.println("Datos enviados correctamente a Firebase.");
    estatus = !estatus;
    digitalWrite(LED_VERDE, estatus);
    Serial.print("Estado LED = ");
    Serial.println(estatus);
  }
  else
  {
    Serial.println("ERROR al enviar datos a Firebase.");
    Serial.print("Razon: ");
    Serial.println(fbdo.errorReason());
  }
  Serial.println("--------------------------------");
}
