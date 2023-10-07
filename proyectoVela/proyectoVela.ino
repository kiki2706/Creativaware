//PROYECTO VELA DE NOEKIKI
//
//A1 Lectura del microfono
//A2 Lectura de la NTC
//D2 Encendido de la lampara

void setup() {
  Serial.begin(9600);
}

void loop() {
  float tensionMicrofono, tensionNTC;
  tensionMicrofono = analogRead(A1);
  tensionNTC = analogRead(A2);
  Serial.println(tensionNTC);
  //La lectura del microfono es un valor de [0-2¹0] y con un fondo de escala de 5v
  //Nuesto valor base (660~) corresponde a 850mV y (770~) a 4V

  //La lectura de la NTC va de 800~ (4v) a 500 (2.3v)

  
  if(tensionNTC < 550){
    digitalWrite(2, HIGH);
  }

  if(tensionMicrofono > 720){
    digitalWrite(2, LOW);
  }
}
