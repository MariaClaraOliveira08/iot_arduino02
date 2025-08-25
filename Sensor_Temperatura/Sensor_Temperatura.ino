//bibliotecas utilizadas
#include <math.h>
#include "AdafruitIO_WiFi.h"

//configurações da rede wifi
#define WIFI_SSID "Maria"
#define WIFI_PASS "11102007"

//autenticação adafruit IO
#define IO_USERNAME "MariaClaraOliveira08"
#define IO_KEY "aio_ApRF47b21jjDtlCIFZLnsm98o5ys"

AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);

#define pinNTC 34  //NTC nome do pino
#define pinLed 14  //pino do led

//controle de envio de dados
float temp_atual = 0;
float temp_anterior = -1;

//variavel / ponteiro para referenciar o feed temperatura
AdafruitIO_Feed* temperatura = io.feed("temperatura");

const float Rfixo = 10000.0;  //resistor do projeto
const float Beta = 3950.0;
const float R0 = 10000.0;        //nominal do NTC
const float T0_Kelvin = 298.15;  //25°C em Kelvin
const float Vcc = 3.3;

void setup() {
  pinMode(pinNTC, INPUT);
  pinMode(pinLed, OUTPUT);

  Serial.begin(115200);

  while (!Serial)
    ;

  Serial.print("Conectando ao Adafruit IO");
  io.connect();

  while (io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  //mostra status de conexão
  Serial.println();
  Serial.println(io.statusText());

  //configuração do callback, quando o feed receber (atualizar) um valor.
  temperatura -> onMessage(handleTemperatura);
  //registra a função de callback
  //ela será chamada sempre que o feed receber um novo dado

  delay(1000);
}

void loop() {

  //manter a conexão com o adafruit IO ativa
  io.run();

  // publicacao(); //chamada da função publish

  delay(3000);
}
