int pinSensorLluvia = 35;   // Pin al que está conectado el sensor HR0101
int pinSensorHumedad = 34;   // Pin al que está conectado el sensor de humedad
int valorLluvia = 0;        
int valorHumedadADC = 0;    
float voltaje = 0;          
float humedad = 0;          
int umbralLluvia = 300;     

void setup() {
  Serial.begin(9600);  // Inicializar la comunicación serial
}

void loop() {
  // Lectura del sensor de lluvia HR0101
  valorLluvia = analogRead(pinSensorLluvia);

  // Lectura del sensor de humedad del suelo
  valorHumedadADC = analogRead(pinSensorHumedad);
  voltaje = valorHumedadADC * (3.3 / 4095.0);
  humedad = (voltaje / 3.3) * 100.0;

  // Enviar los datos por serial en formato de texto
  Serial.print("Lluvia:");
  Serial.print(valorLluvia);
  Serial.print(",Humedad:");
  Serial.print(humedad);
  Serial.println("%");

  delay(1000);  // Esperar 1 segundo antes de la siguiente lectura
}
