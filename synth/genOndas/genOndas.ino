#include "FspTimer.h"
#include "Defines.h"
FspTimer audio_timer;

#include "Sound.h"
#include "Button.h"

<<<<<<< HEAD
<<<<<<< HEAD
#define pot1 A1
#define pot2 A2
#define pot3 A3
#define pot4 A4
#define pot5 A5
=======
=======
>>>>>>> e6bbf93 (synthV1.7: ADSR full operativo)
#define FrecuenciaPin A5
#define AtaquePin A1
#define DecayPin A2
#define SustainPin A3
#define ReleasePin A4
<<<<<<< HEAD
>>>>>>> e6bbf93 (synthV1.7: ADSR full operativo)
=======
>>>>>>> e6bbf93 (synthV1.7: ADSR full operativo)
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
  synthBeginTimer(44100);

  
  synthSetFrecuency(475);
}

void loop() {

<<<<<<< HEAD
<<<<<<< HEAD
  //Ponemos el modo ADSR
  synthADSR(analogRead(pot1),
              analogRead(pot2),
              analogRead(pot3),
              analogRead(pot4));
  synthSetFrecuency(analogRead(pot5));

  //synthSetVolume(analogRead(potxd));

=======
=======
>>>>>>> e6bbf93 (synthV1.7: ADSR full operativo)
  synthADSR(analogRead(AtaquePin),
            analogRead(DecayPin),  
            analogRead(SustainPin),
            analogRead(ReleasePin));
<<<<<<< HEAD
>>>>>>> e6bbf93 (synthV1.7: ADSR full operativo)
=======
>>>>>>> e6bbf93 (synthV1.7: ADSR full operativo)

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
