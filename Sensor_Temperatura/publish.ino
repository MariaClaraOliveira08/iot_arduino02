float lerTemperaturaNTC(int pino, int numLeituras) {  //float é número quebrado
  long somaLeituras = 0; //acumula as leituras

  for (int i = 0; i < numLeituras; i++) {  //i++ soma mais 1
    somaLeituras += analogRead(pino);
    delay(5);
  }

  float leituraMedia = somaLeituras / (float)numLeituras;

  float Vout = leituraMedia * (Vcc / 4095.0);

  //calculo da resistencia
  float Rntc = Rfixo * ((Vcc / Vout) - 1.0);

  float tempK = 1.0 / ((1.0 / T0_Kelvin) + (1.0 / Beta) * log(Rntc / R0));

  return tempK - 273.15;
}

void publicacao(){
  //vai receber o valor da variável lerTemperaturaNTC
  temp_atual = lerTemperaturaNTC(pinNTC, 10);

  //verificando alteração na temperatura
  if (temp_atual == temp_anterior) {
    return;  //esse return aqui vai cancelar a volta do loop
  }

  // Serial.print("Temperatura Analogica: ");
  // Serial.println(analogRead(pinNTC));

  Serial.print(F("Temperatura enviada: "));
  Serial.print(temp_atual, 2);
  Serial.println(F("°C"));
  Serial.print(temp_atual, 2);
  Serial.println(F("°C"));
  //envio / registro no feed "temperatura" no adafruit IO
  temperatura->save(temp_atual);

  temp_anterior = temp_atual;

}