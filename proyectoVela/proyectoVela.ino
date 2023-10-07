//PROYECTO VELA DE NOEKIKI
//
//A1 Lectura del microfono
//A2 Lectura de la NTC
//D2 Encendido de la lampara

//Cuando calor-->se enciende
//Si soplas--> se apaga
//Si soplas y aun hay calor se apaga 

#define EntradaMicro A1
#define EntradaNTC A2
#define rele 2

int medida[2] = {0, 0};
unsigned long delayInicial = 0;
float tensionMicrofono, tensionNTC;


void setup() {
  Serial.begin(9600);
  delayInicial = millis();
  
}

void loop() {
  tensionMicrofono = analogRead(EntradaMicro);
  tensionNTC = analogRead(EntradaNTC);
  //Serial.println(tensionNTC);
  //La lectura del microfono es un valor de [0-2¹0] y con un fondo de escala de 5v
  //Nuesto valor base (660~) corresponde a 850mV y (770~) a 4V
  //La lectura de la NTC va de 800~ (4v) a 500 (2.3v)

  
  if((millis() - delayInicial) >= 250){//Cada 200ms tomamos una medida
    delayInicial = millis();
    
    if((medida[1] == 0) && (medida[0] != 0)){//Si tenemos una sola medida...
       medida[1] = tensionNTC;

        if((medida[0]-medida[1]) > 15)//Si la pendiente decrece no es por un pulso espurio...
            digitalWrite(rele, HIGH);
          else if((medida[1] < 300) && ((medida[0]-medida[1]) < 1))//Si no hay pendiente pero se esta aplicando fuego...
           digitalWrite(rele, HIGH);

         medida[0] = 0;
         medida[1] = 0;
    }
    else//No tenemos ninguna medida
    medida[0] = tensionNTC;
 }
      
  
  
  if(tensionMicrofono > 720){//Siempre que se sople se apagará
    digitalWrite(rele, LOW);
  } 
  
}//void loop
