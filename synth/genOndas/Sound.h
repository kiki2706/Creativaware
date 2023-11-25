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
const uint32_t  SAMPLE_ARRAY_SIZE = 2300;
volatile uint16_t sampleArray[NUMBER_OF_KEYS][SAMPLE_ARRAY_SIZE];//array that save the waveforms   INICIALIZA LA MIERDA ESTA NOE
volatile uint16_t sampleIndex[NUMBER_OF_KEYS] = {0,0,0,0}; // sampler counter
volatile uint16_t NUMBER_OF_SAMPLES[NUMBER_OF_KEYS];//number of samples: calculate using sample time and frecuency

volatile float samplerFrecuency = 44100;
volatile uint16_t SIGNAL_MAX_SIZE = 4096;  // hardware max value
volatile uint16_t LOCAL_SIGNAL_SIZE = SIGNAL_MAX_SIZE >> 4;// max value for each key (then, change 4 by countKeyPressed)
volatile uint16_t newSIGNAL_MAX_SIZE = 4096; // when the max changes
 
volatile uint8_t kindOfWave = 2;
volatile uint8_t keys[NUMBER_OF_KEYS] = {0,0,0,0};
volatile uint8_t countKeysPressed;

volatile uint16_t currentFrecuency = 0;
volatile uint8_t filtroFrecuency = 0;


//-------------------------------------
//    N-KEY STATE PRESSED/UNPRESSED
//-------------------------------------
void synthKeysState(uint8_t pressedKey, uint8_t keyState){
  keys[pressedKey] = keyState;
  uint8_t TEMP_countKeysPressed = 0;
  
  for(uint8_t i = 0; i < NUMBER_OF_KEYS; i++)
    if(keys[i]) TEMP_countKeysPressed++; 

  countKeysPressed = TEMP_countKeysPressed;// semi atomic!!
}


//---------------------------------
//    SET FRECUENCY OSCILLATOR
//---------------------------------
void synthSetFrecuency(uint16_t frecuency){
  if(frecuency != currentFrecuency) filtroFrecuency++;
  if(filtroFrecuency == 3){
  filtroFrecuency = 0;
    
    for(uint8_t i = 0; i < NUMBER_OF_KEYS; i++){
      NUMBER_OF_SAMPLES[i] = samplerFrecuency/(frecuency-(i*20));//CAMBIAR LA RELACION DE LAS FRECUENCIAS
      
      for(uint16_t j = 0; j < NUMBER_OF_SAMPLES[i]; j++){
        if(kindOfWave == 0)// Triangle wave
         if(j <= NUMBER_OF_SAMPLES[i] >> 1) sampleArray[i][j] = (2 * j * (double)(LOCAL_SIGNAL_SIZE / (double)NUMBER_OF_SAMPLES[i]));
           else sampleArray[i][j] = (2 * (NUMBER_OF_SAMPLES[i] - j) * (double)(LOCAL_SIGNAL_SIZE / (double)NUMBER_OF_SAMPLES[i]));
        
        else if(kindOfWave == 1)// Square wave
         if(j <= NUMBER_OF_SAMPLES[i] >> 1) sampleArray[i][j] = 0;
           else sampleArray[i][j] = LOCAL_SIGNAL_SIZE;
           
         else if(kindOfWave == 2)// SawTooth wave
          sampleArray[i][j] = (j * (double)(LOCAL_SIGNAL_SIZE / (double)NUMBER_OF_SAMPLES[i]));
      }//for sampler
    }//for all keys  //AQUI ARRIBA VA LA LOCAL NOEWI ACUERDATE CARAJOS
  }
  currentFrecuency = frecuency;
}

//--------------------------
//    SET SYTH MAX VOLUME 
//---------------------------
void synthSetVolume(int newVolume){
  newSIGNAL_MAX_SIZE = map(newVolume, 0, 1024, 0, DAC_RESOLUTION);
}

//-----------------------------------
//    DIFFERENTS WAVEFORMS SETS
//
//    [0]-> Triangle    [1]-> Square
//    [2]-> SawTooth    [3]-> Sine
//-----------------------------------
void synthSetWaveForm(uint8_t wave){
  kindOfWave = wave;
}

//---------------------------------
//  CALLBACK ISR USED BY TIMER
//---------------------------------
void timer_callback(timer_callback_args_t __attribute((unused)) *p_args) {
  uint16_t lastSample[NUMBER_OF_KEYS], finalSample = 0;

  for(uint8_t i = 0; i < NUMBER_OF_KEYS; i++){
    if(keys[i]){// if i is clicked...
      //
      // when a period finishes...
      //
      sampleIndex[i]++;
      
      if(sampleIndex[i] >= NUMBER_OF_SAMPLES[i]){
         sampleIndex[i] = 0;
         SIGNAL_MAX_SIZE = newSIGNAL_MAX_SIZE;
         LOCAL_SIGNAL_SIZE = (uint16_t)((double)SIGNAL_MAX_SIZE / (double)countKeysPressed);
      }
      lastSample[i] = sampleArray[i][sampleIndex[i]];
    }
    else lastSample[i] = 0;

    
     finalSample += lastSample[i];
  }// for all keys[i]

  
  *DAC12_DADR0 = finalSample;   // DAC update DAC ignores top 4 bits
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
