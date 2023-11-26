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

uint8_t waveForm=0;
bool semaforo = false;

Button botonFormaOnda(FormaOndaPin);
Button tecla1(pinTecla1);
Button tecla2(pinTecla2);
Button tecla3(pinTecla3);

void setup() {
  Serial.begin(9600);
  delay(1000);
  pinMode(LED_BUILTIN, OUTPUT);
  
  //*AGT0_AGTCR = 0; // disable Millis counter, delay etc. don't want this Interrupt messing up output stability

  synthSetFrecuency(100);
  
  // now setup the sound                                                
  synthSetupDac();
  synthBeginTimer(44100);

  //pinMode(2, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  synthSetFrecuency(map(analogRead(FrecuenciaPin), 0, 1024, 20, 3000));

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
  
  //  ADSR EXAMPLE BUTTONs CODE
  /*if (value == HIGH) 
    if(cuenta > 0)
      cuenta = cuenta -1;
    else if(cuenta  == 0);
  else 
    if (cuenta < 10)
      cuenta = cuenta + 1;
    else if (cuenta == 10);*/
}
