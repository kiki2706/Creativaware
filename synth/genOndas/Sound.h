//**********************************************************************
//
//
//                DIY     SYNTH     ARDUINO    BASED
//
//                           :)   ENJOY!!!
//
//  Library developed starting from Grumpy Mike's library PON EL ENLACE NOE
//   and hightly modified by Noé Paniagua and Kiki Gómez, UMA students...
//
//
//**********************************************************************
#include "FspTimer.h"

// CONFIGURATION VALUES
#define NUMBER_OF_KEYS 4
#define SIGNAL_SIZE 4095 //(2pow12 Dac resolution)
#define HALF_SIGNAL_SIZE (SIGNAL_SIZE >> 1)

volatile long int sampleIndex = 0; 
volatile float frecuency = 500;
//volatile float sampleVolume = 1; // full volume
volatile long int NUMBER_OF_SAMPLES = 44100/frecuency;
uint8_t kindOfWave = 0;
int trianguloPendiente = 1;

//uint8_t keys[NUMBER_OF_KEYS];

/*void synthKeysState(uint8_t pressedKey, uint8_t keyState){
  keys[pressedKey] = keyState;
}*/

void synthSetFrecuency(uint32_t frecuency){
  NUMBER_OF_SAMPLES = 44100/frecuency;
}

//---------------------------------
//    DIFFERENTS WAVEFORMS SETS
//---------------------------------
void synthSetTriangle(void){
  kindOfWave = 0;
}

void synthSetSquare(void){
  kindOfWave = 1;
}

void synthSetSawTooth(void){
  kindOfWave = 2;
}

void synthSetSine(void){
  kindOfWave = 3;
}

//---------------------------------
//  CALLBACK ISR USED BY TIMER
//---------------------------------
void timer_callback(timer_callback_args_t __attribute((unused)) *p_args) {
  uint16_t lastSample;
  
  if(sampleIndex >= NUMBER_OF_SAMPLES) sampleIndex = 0;//When period finishes...
  sampleIndex++;
  
  if(kindOfWave == 0){//Triangle wave
    if(sampleIndex <= NUMBER_OF_SAMPLES/2) lastSample = (sampleIndex * (double)(SIGNAL_SIZE / (double)NUMBER_OF_SAMPLES));
    else lastSample = ((NUMBER_OF_SAMPLES - sampleIndex) * (double)(SIGNAL_SIZE / (double)NUMBER_OF_SAMPLES));
  if(kindOfWave == 1)//Square wave
    if(sampleIndex <= NUMBER_OF_SAMPLES/2) lastSample = 0;
    else lastSample = 4095;
  if(kindOfWave == 2)//SawTooth wave
    lastSample = (sampleIndex * (double)(SIGNAL_SIZE / (double)NUMBER_OF_SAMPLES));
  if(kindOfWave == 3)//Sine wave
    lastSample = 0;



  
  *DAC12_DADR0 = lastSample;   // DAC update DAC ignores top 4 bits
} 

//---------------------------------
//      SAMPLER TIMER SETUP
//---------------------------------
bool synthBeginTimer(float rate) {
  uint8_t timer_type = GPT_TIMER;
  int8_t tindex = FspTimer::get_available_timer(timer_type);
  if (tindex < 0){
    tindex = FspTimer::get_available_timer(timer_type, true);
  }
  if (tindex < 0){
    return false;
  }
  FspTimer::force_use_of_pwm_reserved_timer();
  if(!audio_timer.begin(TIMER_MODE_PERIODIC, timer_type, tindex, rate, 0.0f, timer_callback)){
    return false;
  }
  if (!audio_timer.setup_overflow_irq()){
    return false;
  }
  if (!audio_timer.open()){
    return false;
  }
  if (!audio_timer.start()){
    return false;
  }
  return true;
}

//---------------------------------
//SETUP ARDUINO R4 DAC--> PIN A0
//---------------------------------
void synthSetupDac(void)       // Note make sure ADC is stopped before setup DAC
  {
  *MSTP_MSTPCRD &= ~(0x01 << MSTPD20);  // Enable DAC12 module
  *DAC12_DADPR    = 0x00;               // DADR0 Format Select Register - Set right-justified format
//  *DAC12_DAADSCR  = 0x80;             // D/A A/D Synchronous Start Control Register - Enable
  *DAC12_DAADSCR  = 0x00;               // D/A A/D Synchronous Start Control Register - Default
// 36.3.2 Notes on Using the Internal Reference Voltage as the Reference Voltage
  *DAC12_DAVREFCR = 0x00;               // D/A VREF Control Register - Write 0x00 first - see 36.2.5
  *DAC12_DADR0    = 0x0000;             // D/A Data Register 0 
   delayMicroseconds(10);               // Needed delay - see data sheet
  *DAC12_DAVREFCR = 0x01;               // D/A VREF Control Register - Select AVCC0/AVSS0 for Vref
  *DAC12_DACR     = 0x5F;               // D/A Control Register - 
   delayMicroseconds(5);                // Needed delay - see data sheet
  *DAC12_DADR0    = 2048;               // D/A Data Register 0 - value of mid range bias  
  *PFS_P014PFS   = 0x00000000;          // Port Mode Control - Make sure all bits cleared
  *PFS_P014PFS  |= (0x1 << 15);         // ... use as an analog pin
  }
