 //bibliotecas utilizadas
#include <math.h>
#include "AdafruitIO_WiFi.h"

//configurações da rede wifi
#define WIFI_SSID ""
#define WIFI_PASS ""

//autenticação adafruit IO
#define IO_USERNAME "MariaClaraOliveira08"
#define IO_KEY "aio_FwCw335pAdh3QFwlsYb81Rw1naFm"

AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);

#define pinNTC 34 //NTC nome do pino

//controle de envio de dados
float temp_atual = 0;
float temp_anterior = -1;

//variavel / ponteiro para referenciar o feed temperatura
 AdafruitIO_Feed * temperatura = io.feed("temperatura");

const float Rfixo = 10000.0; //resistor do projeto
const float Beta = 3950.0;
const float R0 = 10000.0; //nominal do NTC
const float T0_Kelvin = 298.15; //25°C em Kelvin
const float Vcc = 3.3;

float lerTemperaturaNTC(int pino, int numLeituras){ //float - número quebrado
  long somaLeituras = 0;

  for(int i = 0; i < numLeituras; i++){ //i++ soma mais 1
    somaLeituras += analogRead(pino);
    delay(5);
  } 

  float leituraMedia = somaLeituras / (float)numLeituras;

  float Vout = leituraMedia *(Vcc / 4095.0);

 //calculo da resistencia
  float Rntc = Rfixo * ((Vcc / Vout) - 1.0);

  float tempK = 1.0 / ((1.0 / T0_Kelvin) + (1.0 / Beta) * log(Rntc / R0));

  return tempK - 273.15;
}

void setup() {
 pinMode(pinNTC, INPUT);

 Serial.begin(115200);

 while(!Serial);

 Serial.print("Conectando ao Adafruit IO");
 io.connect();

 while(io.status() < AIO_CONNECTED){
  Serial.print(".");
  delay(500);
 }

 Serial.println();
 Serial.println(io.statusText());

 delay(1000);
}

void loop() {

  //manter a conexão com o adafruit IO ativa
  io.run();

  //vai receber o valor da variável lerTemperaturaNTC
  temp_atual = lerTemperaturaNTC(pinNTC, 10);

  //verificando alteração na temperatura
  if(temp_atual == temp_anterior){
    return; //esse return aqui vai cancelar a volta do loop
  }

  // Serial.print("Temperatura Analogica: ");
  // Serial.println(analogRead(pinNTC));

  Serial.print(F("Temperatura enviada: "));
  Serial.print(temp_atual, 2);
  Serial.println(F("°C"));

  //envio / registro no feed "temperatura" no adafruit IO
  temperatura -> save(temp_atual);

  temp_anterior = temp_atual;

  delay(3000);
}
