//PROYECTO VELA DE NOEKIKI
//
//A1 Lectura del microfono
//A2 Lectura de la NTC
//D2 Encendido de la lampara

//Cuando calor-->se enciende
//Si soplas--> se apaga
//Si soplas y aun hay calor se apaga 

#define EntradaMicro A2
#define EntradaNTC A1
#define rele 11

int medida[2] = {0, 0};
unsigned long delayInicial = 0;
int pendientePrevia = 0, pendienteActual = 0;
float tensionMicrofono, tensionNTC;


void setup() {
  Serial.begin(9600);
  delayInicial = millis();
  pinMode(11, OUTPUT);
  
}

void loop() {
  tensionMicrofono = analogRead(EntradaMicro);
  tensionNTC = analogRead(EntradaNTC);
  //La lectura del microfono es un valor de [0-2¹0] y con un fondo de escala de 5v
  //Nuesto valor base (660~) corresponde a 850mV y (770~) a 4V
  //La lectura de la NTC va de 800~ (4v) a 500 (2.3v)

  
  if((millis() - delayInicial) >= 100){//Cada 200ms tomamos una medida
    delayInicial = millis();
    
    if((medida[1] == 0) && (medida[0] != 0)){//Si tenemos una sola medida...
       medida[1] = tensionNTC;

       pendienteActual = medida[0]-medida[1];
      //Serial.println(pendienteActual);
      //Serial.println(tensionNTC);
      //Serial.println("/////////////////");
       if(pendientePrevia < pendienteActual){
          if(pendienteActual > 7)//Si la pendiente decrece no es por un pulso espurio...
              digitalWrite(rele, HIGH);
          else if((medida[1] < 600) && (pendienteActual > 5))
              digitalWrite(rele, HIGH);
          else if((medida[1] < 350) && (pendienteActual > 3))
              digitalWrite(rele, HIGH);
          else if((medida[1] < 150) && (pendienteActual > 2))//Si no hay pendiente pero se esta aplicando fuego...
              digitalWrite(rele, HIGH);
        }

        medida[0] = 0;
        medida[1] = 0;
        pendientePrevia = pendienteActual;
    }
    else//No tenemos ninguna medida
    medida[0] = tensionNTC;
  
 }
      
  
  
  if(tensionMicrofono > 720){//Siempre que se sople se apagará
    digitalWrite(rele, LOW);
  } 
  
}//void loop
