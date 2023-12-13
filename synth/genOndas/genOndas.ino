#include "FspTimer.h"
#include "Defines.h"
FspTimer audio_timer;

#include "Sound.h"
#include "Button.h"


#define FrecuenciaPin A5
#define AtaquePin A1
#define DecayPin A2
#define SustainPin A3
#define ReleasePin A4
#define FormaOndaPin 2
#define pinTecla1 3
#define pinTecla2 4
#define pinTecla3 5
#define pinTecla4 6

uint8_t waveForm=0;
bool semaforo = false;

Button botonFormaOnda(FormaOndaPin);
Button tecla1(pinTecla1);
Button tecla2(pinTecla2);
Button tecla3(pinTecla3);
Button tecla4(pinTecla4);

bool nadaPulsado = true;

void setup() {
  delay(500);
  
  //*AGT0_AGTCR = 0; // disable Millis counter, delay etc. don't want this Interrupt messing up output stability
  
  // now setup the sound                   
  synthSetupDac();
  synthBeginTimer(40000);
  synthSetFrecuency(475);
}

void loop() {
  //Ponemos el modo ADSR
  synthSetFrecuency(analogRead(FrecuenciaPin));
  synthADSR(analogRead(AtaquePin),
            analogRead(DecayPin),  
            analogRead(SustainPin),
            analogRead(ReleasePin));

  //  
  // set the current waveform clicking the button
  //
  if (botonFormaOnda.isPressed()){
      if(!semaforo){
       semaforo = true;
       waveForm++;
        if(waveForm >= 3) waveForm = 0;
        synthSetWaveForm(waveForm);
      }
  }   
  else semaforo = false; 

  //
  // each key plays a different wave
  //
  synthKeysState(0, tecla1.isPressed());

  synthKeysState(1, tecla2.isPressed());
  
  synthKeysState(2, tecla3.isPressed());

  synthKeysState(3, tecla4.isPressed());

//PONER EN UNA FUNCION EN ALGUN LAO XDIOS
//
// La sabedora de cuantas teclas estansiendotocadas punto com
//
uint8_t TEMP_countKeysPressed = 0;
   for(uint8_t i = 0; i < NUMBER_OF_KEYS; i++)
     if(keys[i] || adsr[i].semRelease) TEMP_countKeysPressed++; 

    if(!TEMP_countKeysPressed)
      countKeysPressed = 1;// semi atomic!!
    else
      countKeysPressed = TEMP_countKeysPressed;

}//main
