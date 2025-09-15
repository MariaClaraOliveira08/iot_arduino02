//bibliotecas utilizadas
#include <math.h>
#include "AdafruitIO_WiFi.h"
#include "NewPing.h"

//configurações da rede wifi
#define WIFI_SSID "Maria"
#define WIFI_PASS "11102007"

//autenticação adafruit IO
#define IO_USERNAME "MariaClaraOliveira08"
#define IO_KEY "aio_VQZI262f4G8A6Meoo7wDSn4NsEgN"

AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);

#define pinNTC 34  //NTC nome do pino

#define BUZZER_PIN 27
#define LED_ALARME 13
#define BOTAO_FISICO 26
#define TRIG_PIN 12
#define ECHO_PIN 14

//Configurações ultrassonico
#define MAX_DISTANCE 100
NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);

//controle de envio de dados
float temp_atual = 0;
float temp_anterior = -1;

//variavel / ponteiro para referenciar o feed temperatura
// AdafruitIO_Feed* temperatura = io.feed("temperatura");
AdafruitIO_Feed* botaoalarme = io.feed("botaoalarme");


#define pinNTC 34
#define pinLed 14  //pino do led

const float Rfixo = 10000.0;  //resistor do projeto
const float Beta = 3950.0;
const float R0 = 10000.0;        //nominal do NTC
const float T0_Kelvin = 298.15;  //25°C em Kelvin
const float Vcc = 3.3;

//Definição das variaveis de controle
bool alarmeAtivo = false;
unsigned int distancia = 0;
const int LIMITE_DISTANCIA = 15;

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_ALARME, OUTPUT);
  pinMode(BOTAO_FISICO, OUTPUT);

  Serial.begin(115200);

  while (!Serial)
    ;

  pinMode(pinNTC, INPUT);
  pinMode(pinLed, OUTPUT);

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
  // temperatura -> onMessage(handleTemperatura);
  //registra a função de callback
  botaoalarme->onMessage(handleAlarme);
  //ela será chamada sempre que o feed receber um novo dado

  Serial.println("Solicitando o estado inicial do alarme: ");
  botaoalarme->get();
}

void loop() {

  //manter a conexão com o adafruit IO ativa
  io.run();

  //leitura do botão fisico
  if (digitalRead(BOTAO_FISICO) == 1) {
    delay(200);  //debounce simples (atraso)
    alarmeAtivo = !alarmeAtivo;

    botaoalarme->save(String(alarmeAtivo ? "true" : "false"));
    Serial.println(alarmeAtivo ? F("Alarme ARMADO pelo botao fisico") : F("Alarme DESARMADO pelo botao fisico"));
  }

  // publicacao(); //chamada da função publish

  distancia = sonar.ping_cm();
  Serial.print(F("Distancia Lida: "));
  Serial.println(distancia);
  Serial.println(" cm");

  //ativação ou desativação do alarme
  if(alarmeAtivo && distancia > 0 && distancia < LIMITE_DISTANCIA){
    ativarAlerta();
  }
  else{
    desligarAlerta();
  }
}
