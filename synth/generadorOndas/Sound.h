volatile float sampleIndex = 0.0;
float sampleIncrement = 1.0; // sets frequency of audio output
volatile float sampleVolume = 0.1; // full volume
// this defines the note intervals in the scale of C major
int8_t scale[] = { 0, 2, 4, 5, 7, 9, 11, 12 }; 

#define ENVELOPE_TABLE_LEN 256
volatile float envelope_table[ENVELOPE_TABLE_LEN]; 
volatile boolean stopNote = false;
int16_t waveTable [1024];

void generateTable(void){
  const float pi = 3.1415;

  for(int16_t i = 0; i<1024; i++){ // generate the waveform
    // Ruido waveTable[i] = random(0, 1024);
    waveTable[i] = i;
  } 

  
}

// callback ISR used by timer
void timer_callback(timer_callback_args_t __attribute((unused)) *p_args) {
  static int16_t lastSample = 0;   

  sampleIndex += sampleIncrement; 
  if(sampleIndex > 1023.0) sampleIndex -= 1023.0;
  lastSample = (sampleVolume * waveTable[int16_t(sampleIndex)]);
  *DAC12_DADR0 = lastSample;   // DAC update DAC ignores top 4 bits
} // end of callback ISR

bool beginTimer(float rate) {
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

int potenciometro(int pot){
  sampleIncrement = pot;
  return sampleIncrement;
}

void setup_dac(void)       // Note make sure ADC is stopped before setup DAC
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
