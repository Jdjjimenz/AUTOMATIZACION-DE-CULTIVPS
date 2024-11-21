#include <WiFi.h>
#include <ThingSpeak.h>

// Configura tu red WiFi
const char* ssid = "Redmi 9";              // Reemplaza con tu SSID
const char* password = "17846547";         // Reemplaza con tu contraseña

// Configura ThingSpeak
unsigned long myChannelNumber = 2690504;   // Reemplaza con tu número de canal
const char* myWriteAPIKey = "G2OFXTCYEE59ABTJ"; // Reemplaza con tu API Key

// Inicializa el cliente WiFi
WiFiClient client;

// Configuración del sensor de lluvia
int pinSensorLluvia = 35;       // Pin analógico donde está conectado el sensor de lluvia
int valorADC = 0;               // Variable para almacenar el valor leído del ADC
int umbralLluvia = 300;         // Umbral para determinar si está lloviendo

// Configuración del sensor de nivel de líquido
int pinSensorLiquido = 33;      // Pin donde está conectado el sensor de nivel de líquido
int liquidLevel = 0;            // Variable para el nivel de líquido

// Configuración del sensor de humedad
int pinSensorHumedad = 34;      // Pin analógico donde está conectado el sensor de humedad
float humedad = 0;              // Variable para almacenar el porcentaje de humedad
float voltaje = 0;              // Variable para almacenar el voltaje calculado

void setup() {
  Serial.begin(9600);           // Inicializar la comunicación serial
  pinMode(pinSensorLluvia, INPUT);  // Pin del sensor de lluvia
  pinMode(pinSensorLiquido, INPUT); // Pin del sensor de líquido
  pinMode(pinSensorHumedad, INPUT); // Pin del sensor de humedad

  // Conectar a Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando a WiFi...");
  }
  Serial.println("Conectado a WiFi");

  // Iniciar ThingSpeak
  ThingSpeak.begin(client);
}

void loop() {
  // Leer el valor del sensor de lluvia (0 a 4095)
  valorADC = analogRead(pinSensorLluvia);
  Serial.print("Valor del sensor de lluvia: ");
  Serial.println(valorADC);

  String estadoLluvia;
  if (valorADC < umbralLluvia) {  // Si el valor es menor que el umbral
    estadoLluvia = "Lluvia";
  } else {
    estadoLluvia = "Sin Lluvia";
  }

  // Leer el nivel de líquido (0 o 1)
  liquidLevel = digitalRead(pinSensorLiquido);
  Serial.print("Nivel de líquido: ");
  Serial.println(liquidLevel);

  // Leer el valor del sensor de humedad (0 a 4095)
  valorADC = analogRead(pinSensorHumedad);
  voltaje = valorADC * (3.3 / 4095.0);
  humedad = (voltaje / 3.3) * 100.0;

  Serial.print("Humedad del suelo: ");
  Serial.print(humedad);
  Serial.println(" %");

  // Enviar los datos a ThingSpeak
  ThingSpeak.setField(1, valorADC);           // Enviar valor del sensor de lluvia al campo 1
  ThingSpeak.setField(2, estadoLluvia);      // Enviar estado de lluvia al campo 2
  ThingSpeak.setField(3, liquidLevel);       // Enviar nivel de líquido al campo 3
  ThingSpeak.setField(4, humedad);           // Enviar humedad del suelo al campo 4

  int httpResponseCode = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if (httpResponseCode == 200) {
    Serial.println("Datos enviados exitosamente.");
  } else {
    Serial.print("Error al enviar datos. Código de respuesta: ");
    Serial.println(httpResponseCode);
  }

  // Espera 10 segundos antes de la próxima lectura
  delay(10000);
}

