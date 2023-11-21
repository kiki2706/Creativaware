//**********************************************************************
//
//
//                DIY     SYNTH     ARDUINO    BASED
//
//                           :)   ENJOY!!!
//
//  Library developed starting from Grumpy Mike's code PON EL ENLACE NOE
//   and hightly modified by Noé Paniagua and Kiki Gómez, UMA students.
//
//
//**********************************************************************
#include "FspTimer.h"

//-------------------------------------
//    SYNTH HARDWARE CONFIGURATIONS
//-------------------------------------
#define NUMBER_OF_KEYS 4
#define DAC_RESOLUTION 4095

//-------------------------------
//    SYNTH SOFTWARE VARIABLES
//-------------------------------
volatile float samplerFrecuency = 44100;
volatile float sampleVolume = 1;
volatile float frecuency = 500;
volatile int SIGNAL_SIZE = 4096;
volatile int newSIGNAL_SIZE = 4096;

volatile long int NUMBER_OF_SAMPLES = samplerFrecuency/frecuency;//
volatile long int sampleIndex = 0; 
 
uint8_t kindOfWave = 0;
uint8_t keys[NUMBER_OF_KEYS];


//-------------------------------------
//    N-KEY STATE PRESSED/UNPRESSED
//-------------------------------------
void synthKeysState(uint8_t pressedKey, uint8_t keyState){
  keys[pressedKey] = keyState;
}


//---------------------------------
//    SET FRECUENCY OSCILLATOR
//---------------------------------
void synthSetFrecuency(uint32_t frecuency){
  NUMBER_OF_SAMPLES = samplerFrecuency/frecuency;
}

//----------------------
//    SET SYTH VOLUME
//----------------------
void synthSetVolume(int newVolume){
  newSIGNAL_SIZE = map(newVolume, 0, 1024, 0, DAC_RESOLUTION);
}

//-----------------------------------
//    DIFFERENTS WAVEFORMS SETS
//
//    [0]-> Triangle    [1]-> Square
//    [2]-> SawTooth    [3]-> Sine
//-----------------------------------
void synthSetWaveForm(int wave){
  kindOfWave = wave;
}

//---------------------------------
//  CALLBACK ISR USED BY TIMER
//---------------------------------
void timer_callback(timer_callback_args_t __attribute((unused)) *p_args) {
  uint16_t lastSample;
  
  if(sampleIndex >= NUMBER_OF_SAMPLES){
    sampleIndex = 0;//When period finishes...
    SIGNAL_SIZE = newSIGNAL_SIZE;
  }
  sampleIndex++;
  
  if(kindOfWave == 0)// Triangle wave
    if(sampleIndex <= NUMBER_OF_SAMPLES/2) lastSample = (2 * sampleIndex * (double)(SIGNAL_SIZE / (double)NUMBER_OF_SAMPLES));
      else lastSample = (2 * (NUMBER_OF_SAMPLES - sampleIndex) * (double)(SIGNAL_SIZE / (double)NUMBER_OF_SAMPLES));
  else if(kindOfWave == 1)// Square wave
    if(sampleIndex <= NUMBER_OF_SAMPLES/2) lastSample = 0;
      else lastSample = SIGNAL_SIZE;
  else if(kindOfWave == 2)// SawTooth wave
    lastSample = (sampleIndex * (double)(SIGNAL_SIZE / (double)NUMBER_OF_SAMPLES));
  else if(kindOfWave == 3)//Sine wave
    lastSample = 0;// XD por lo pront
  
  *DAC12_DADR0 = lastSample;   // DAC update DAC ignores top 4 bits
} 

//---------------------------------
//      SAMPLER TIMER SETUP
//---------------------------------
bool synthBeginTimer(float rate) {
  uint8_t timer_type = GPT_TIMER;
  int8_t tindex = FspTimer::get_available_timer(timer_type);

  samplerFrecuency = rate;
  
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
