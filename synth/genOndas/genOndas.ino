/*
  Example developed starting from Toby Oxborrow's sketch
  https://github.com/tobyoxborrow/gameoflife-arduino/blob/master/GameOfLife.ino
   adding an extra pattern 
   and turning lit LEDs into notes - Grumpy Mike
   Wave D/A output thanks to susan-parker Triode Girl https://github.com/TriodeGirl/Arduino_Direct_Register_Operations
   Each pattern is scanned and turned into notes. The first 6 LEDs
   in the X axis cover 6 octaves and the next 6 positions repeat this pattern.
   
   DO NOT CONNECT A LOUDSPEAKER TO AN ARDUINO R4 - YOU WILL DAMAGE THE ARDUINO
   You need to use an amplifier. 
*/
#include "FspTimer.h"
#include "Defines.h"

FspTimer audio_timer;
#include "Sound.h"

void setup() {
  // first, setup serial port
  Serial.begin(115200);
  delay(1000);
  *AGT0_AGTCR = 0; // disable Millis counter, delay etc. don't want this Interrupt messing up output stability

  
  // now setup the sound                                                
  synthSetupDac();
  synthBeginTimer(44100); // sets the sample rate as CD quality
}

void loop() {
  int entrada1 = analogRead(A1);
  int pot = map(entrada1, 0, 1024, 20, 3000);

  synthSetFrecuency(pot);
  
  //int muestra = potenciometro(pot);
  //int muestra = potenciometro(map(pot, 0,1024, 1, 20));
  //Serial.println((double)1/NUMBER_OF_SAMPLES, 5);
}
