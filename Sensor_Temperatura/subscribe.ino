//função chamada automaticamente quando o feed receber atualizações
void handleTemperatura(AdafruitIO_Data* data) {
  float temp = data->toFloat();

  Serial.print(F("Temperatura do feed: "));
  Serial.print(temp, 2);
  Serial.println(F("°C"));

  //lógica para acender ou não o LED
  if(temp <= 0){
    digitalWrite(pinLed, HIGH);
  }
  else{
    digitalWrite(pinLed, LOW);
  }
}