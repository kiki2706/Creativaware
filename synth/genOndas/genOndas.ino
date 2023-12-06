#include "FspTimer.h"
#include "Defines.h"
FspTimer audio_timer;

#include "Sound.h"
#include "Button.h"

#define FrecuenciaPin A1
#define VolumenPin A2
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

void setup() {
  Serial.begin(9600);
  delay(500);
  
  //*AGT0_AGTCR = 0; // disable Millis counter, delay etc. don't want this Interrupt messing up output stability

  synthSetFrecuency(100);
  
  // now setup the sound   
  synthADSR(1000,1000,256,1000);                 
  synthSetupDac();
  synthBeginTimer(44100);
}

void loop() {
  
  synthSetFrecuency(map(analogRead(FrecuenciaPin), 0, 1024, 100, 3000));

  synthSetVolume(analogRead(VolumenPin));

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

    if(!TEMP_countKeysPressed)countKeysPressed = 1;// semi atomic!!
    else countKeysPressed = TEMP_countKeysPressed;




    Serial.println(countKeysPressed);
}
