#include "FspTimer.h"
#include "Defines.h"
FspTimer audio_timer;

#include "Sound.h"

#define FrecuenciaPin A1
#define VolumenPin A2
#define FormaOndaPin 2

int frec, vol;

uint8_t value=0, waveForm=0;
bool semaforo = false;

void setup() {
  Serial.begin(9600);
  delay(1000);
  
  *AGT0_AGTCR = 0; // disable Millis counter, delay etc. don't want this Interrupt messing up output stability
  
  // now setup the sound                                                
  synthSetupDac();
  synthBeginTimer(37000); // sets the sample rate as CD quality

  pinMode(2, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  frec = analogRead(FrecuenciaPin);
  frec = map(frec, 0, 1024, 20, 3000);
    synthSetFrecuency(frec);

  vol = analogRead(VolumenPin);
  synthSetVolume(vol);


  value = digitalRead(2);
  
  if (!value) {
    if(!semaforo){
      semaforo = true;
      waveForm++;
      if(waveForm >= 3) waveForm = 0;
      synthSetWaveForm(waveForm);
    }}
  else semaforo = false; 
  
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
