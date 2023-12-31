//****************************************************************************************
//
//
//                        DIY     SYNTH     ARDUINO    BASED
//
//                                  :)   ENJOY!!!
//
//  Library developed starting from Grumpy Mike's code PON EL ENLACE NOE  JJAJJDJJFDHSJDH
//   and hightly modified by Noé Paniagua and Kiki Gómez, Electronica Creativita students.
//
//
//**************************************************************************************
#include "FspTimer.h"

//-------------------------------------
//    SYNTH HARDWARE CONFIGURATIONS
//-------------------------------------
#define NUMBER_OF_KEYS 13
#define DAC_RESOLUTION 4090

//-------------------------------
//    SYNTH SOFTWARE VARIABLES
//-------------------------------
const uint16_t LOOPER_SIZE = 800;
uint16_t looper[LOOPER_SIZE]; 
volatile uint8_t activeLooper = 0, subLooperTimer=0;
volatile uint16_t looperCount = 0;

#define NUMBER_OF_FRECS 52
static uint16_t frecuenciasAfinadas[52] = {385,363, 343, 323, 305, 
                                  288, 272, 257, 242, 229, 
                                  216, 204, 192, 181, 171, 
                                  161, 152, 144, 136, 128, 
                                  121, 114, 108, 102, 96, 
                                  90, 85, 80, 76, 72, 
                                  68, 64, 60, 57, 54, 
                                  51, 48, 45, 42, 40,
                                  38, 36, 34, 32, 30, 28,
                                  27 ,25, 24, 22, 21, 20};
const uint32_t  SAMPLE_ARRAY_SIZE = 200;
static uint16_t sampleArray[NUMBER_OF_KEYS][SAMPLE_ARRAY_SIZE];//array that save the waveforms   INICIALIZA LA MIERDA ESTA NOE
static volatile uint16_t sampleIndex[NUMBER_OF_KEYS] = {0,0,0,0}; // sampler counter
static volatile uint16_t NUMBER_OF_SAMPLES[NUMBER_OF_KEYS];//number of samples: calculate using sample time and frecuency

static volatile uint32_t samplerFrecuency = 700;
static volatile uint16_t SIGNAL_MAX_SIZE = 512;  // hardware max value
static volatile uint16_t newSIGNAL_MAX_SIZE = 512; // hardware max readed value
static volatile uint16_t LOCAL_SIGNAL_SIZE = 512;// max value for each key (then, change 4 by countKeyPressed)

static volatile uint8_t kindOfWave = 0;
static volatile uint8_t keys[NUMBER_OF_KEYS] = {0,0,0,0};
static volatile uint8_t lastKeys[NUMBER_OF_KEYS] = {0,0,0,0};
static uint8_t countKeysPressed;

static uint16_t currentFrecuency = 0;
static uint8_t filtroFrecuency = 0;

//*********ADSR VARIABLES**********
#define MAX_TIME 1

typedef struct ADSR{
  uint8_t semAttack;
  uint8_t semDecay;
  uint8_t semSustain;
  uint8_t semRelease;
  uint16_t timingHighFrec;
  float mod;
};

static ADSR adsr[NUMBER_OF_KEYS];


typedef struct ENVELOPE{
  float resolution;
};
static ENVELOPE envelopeAttack;
static ENVELOPE envelopeDecay;
static float envelopeSustainMod;
static ENVELOPE envelopeRelease;

float mapf(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

//-------------------------------------
//    N-KEY STATE PRESSED/UNPRESSED
//-------------------------------------
void synthKeysState(uint8_t pressedKey, uint8_t keyState){
  keys[pressedKey] = keyState;

  if(lastKeys[pressedKey] != keys[pressedKey]){
    if(keys[pressedKey] == 1){ 
      adsr[pressedKey].semAttack = 1; 
      adsr[pressedKey].semRelease = 0;
      adsr[pressedKey].semDecay = 0;
      adsr[pressedKey].mod = 0;
      
    }
    else {
      adsr[pressedKey].semRelease = 1; 
    }
  }

  lastKeys[pressedKey] = keys[pressedKey];
}


//-------------------------------------
//    ACTIVE LOOPER SAVE
//-------------------------------------
void synthActiveLooper(uint8_t xd){
  looperCount = 0;
  activeLooper = 1;
}


//-------------------
//    ADSR REFRESH
//-------------------
void synthADSR(uint16_t AttackPot, uint16_t DecayPot, uint16_t SustainPot, uint16_t ReleasePot){
  float tempTime, nSteps = 0;
  
  //
  //ATTACK
  //
  tempTime = mapf(AttackPot, 0, 1024, 0.1, MAX_TIME);
  nSteps = (float)tempTime / (float)(0.01);
  envelopeAttack.resolution = (float)1/(float)(nSteps);
  
  //
  //SUSTAIN
  //
  envelopeSustainMod = mapf(SustainPot, 0 , 1024, 0.1, 1);

  //
  //DECAY
  //
  tempTime = mapf(DecayPot, 0, 1024, 0.1, MAX_TIME);
  nSteps = (float)tempTime / (float)(0.01);
  envelopeDecay.resolution = (1-envelopeSustainMod)/(float)(nSteps);
  
  //
  //RELEASE
  //
  tempTime = mapf(ReleasePot, 0, 1024, 0.1, MAX_TIME);
  nSteps = (float)tempTime / (float)(0.01);
  envelopeRelease.resolution = (envelopeSustainMod)/(float)(nSteps);
}

//-----------------------------------------------------------
//    SET FRECUENCY OSCILLATOR (recalculate waveform tables)
//-----------------------------------------------------------
void synthSetFrecuency(uint16_t frecuency){
  static uint8_t frecuencySample;
  if(frecuency != currentFrecuency) filtroFrecuency++;
  
  if(filtroFrecuency == 3){
    filtroFrecuency = 0;

    frecuencySample = map(frecuency , 0, 1024, 0, NUMBER_OF_FRECS - NUMBER_OF_KEYS);//script matlab
    
    for(uint8_t i = 0; i < NUMBER_OF_KEYS; i++){
      NUMBER_OF_SAMPLES[i] = frecuenciasAfinadas[frecuencySample+i];//CAMBIAR LA RELACION DE LAS FRECUENCIAS
      
      for(uint16_t j = 0; j < NUMBER_OF_SAMPLES[i]; j++){
        if(kindOfWave == 0)// Triangle wave
         if(j <= NUMBER_OF_SAMPLES[i] >> 1) sampleArray[i][j] = (2 * j * (float)(LOCAL_SIGNAL_SIZE / (float)NUMBER_OF_SAMPLES[i]));
           else sampleArray[i][j] = (2 * (NUMBER_OF_SAMPLES[i] - j) * (float)(LOCAL_SIGNAL_SIZE / (float)NUMBER_OF_SAMPLES[i]));
        
        else if(kindOfWave == 1)// Square wave
         if(j <= NUMBER_OF_SAMPLES[i] >> 1) sampleArray[i][j] = 0;
           else sampleArray[i][j] = LOCAL_SIGNAL_SIZE;
           
         else if(kindOfWave == 2)// SawTooth wave
          sampleArray[i][j] = (j * (uint16_t)(LOCAL_SIGNAL_SIZE / (float)NUMBER_OF_SAMPLES[i]));
          
      }//for sampler
    }//for all keys  
  }
  currentFrecuency = frecuency;
}

//--------------------------
//    SET SYTH MAX VOLUME 
//---------------------------
void synthSetVolume(int newVolume){
  newSIGNAL_MAX_SIZE = map(newVolume, 0, 1024, 0, DAC_RESOLUTION>>3);
}

//-----------------------------------
//    DIFFERENTS WAVEFORMS SETS
//
//    [0]-> Triangle    [1]-> Square
//    [2]-> SawTooth    [3]-> XD(sine uwu)NOT YET
//-----------------------------------
void synthSetWaveForm(uint8_t wave){
  kindOfWave = wave;
}

//---------------------------------
//  CALLBACK ISR USED BY TIMER
//---------------------------------
void timer_callback(timer_callback_args_t __attribute((unused)) *p_args) {
  uint16_t lastSample[NUMBER_OF_KEYS], finalSample = 0, looperTemp = 0;

  for(uint8_t i = 0; i < NUMBER_OF_KEYS; i++){  
      
    if(keys[i] || adsr[i].semRelease){// if clicked...
      sampleIndex[i]++;
      
      if(sampleIndex[i] >= NUMBER_OF_SAMPLES[i]){//if a period finishes
         sampleIndex[i] = 0;
         SIGNAL_MAX_SIZE = newSIGNAL_MAX_SIZE;
         //LOCAL_SIGNAL_SIZE = (uint16_t)((float)SIGNAL_MAX_SIZE / (float)countKeysPressed);
         LOCAL_SIGNAL_SIZE = SIGNAL_MAX_SIZE;
      }
      //
      //HUGE TIMERERERER
      //
      if(adsr[i].timingHighFrec++ == 400){//10ms
         adsr[i].timingHighFrec = 0;

         //
         // ATTACK
         //
         if(adsr[i].semAttack){
           adsr[i].mod += envelopeAttack.resolution;
           
           if(adsr[i].mod >= 1){//finish attack
            adsr[i].semAttack = 0;
            adsr[i].semDecay = 1;
           }
         }

         //
         // DECAY
         //
         else if(adsr[i].semDecay){
          adsr[i].mod -= envelopeDecay.resolution;
          
          if(adsr[i].mod <= envelopeSustainMod){//finish decay
            adsr[i].semDecay = 0;
          }
        }
        
        //
        // RELEASE
        // 
        else if(adsr[i].semRelease){//if release
          adsr[i].mod -= envelopeRelease.resolution;
          
          if(adsr[i].mod <= 0){//finish decay
            adsr[i].semRelease = 0;
            adsr[i].mod = 0;
          }
        }    
      }//end huge timer

      lastSample[i] = (uint16_t)(sampleArray[i][sampleIndex[i]]*(float)adsr[i].mod);
    }
    else lastSample[i] = 0;

  if(subLooperTimer++ == 128){
    subLooperTimer = 0;
    looperCount++;
    
    if(activeLooper){
      looper[looperCount] = finalSample + looperTemp; 
    }
    else{
      looperTemp = looper[looperCount];
    }

    if(looperCount >= LOOPER_SIZE){
      looperCount = 0;
      activeLooper = 0;
    }
  }
    finalSample += lastSample[i];
  }// for all keys[i]

  
  *DAC12_DADR0 = finalSample;  
} 

//---------------------------------
//      SAMPLER TIMER SETUP
//---------------------------------
bool synthBeginTimer(float rate) {
  uint8_t timer_type = GPT_TIMER;
  int m =0;
  for(m = 0; m < LOOPER_SIZE; m++) looper[m] = 0;
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
