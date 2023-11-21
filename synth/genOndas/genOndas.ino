#include "FspTimer.h"
#include "Defines.h"
FspTimer audio_timer;

#include "Sound.h"

int frec, vol;

void setup() {
  *AGT0_AGTCR = 0; // disable Millis counter, delay etc. don't want this Interrupt messing up output stability

  
  // now setup the sound                                                
  synthSetupDac();
  synthBeginTimer(37000); // sets the sample rate as CD quality
}

void loop() {
  frec = analogRead(A1);
  frec = map(frec, 0, 1024, 20, 3000);
    synthSetFrecuency(frec);

  vol = analogRead(A2);
  synthSetVolume(vol);
}
