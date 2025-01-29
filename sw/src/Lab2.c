// ----------------------------------------------------------------------
//
// Lab2.c
// Runs on TM4C123
// 
// Jonathan Valvano
// December 27, 2024
//
//  Three possible ADC inputs are PE4, PE5, or PE1.      
// ----------------------------------------------------------------------

#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "../inc/ADCSWTrigger.h"
#include "../inc/LaunchPad.h"
#include "../inc/PLL.h"
#include "../inc/Timer2A.h"
#include "../inc/CortexM.h"
#include "../inc/UART.h"
#include "../inc/SSD1306.h"
// bit-specific addresses for PC7 and PC6
#define PC7   (*((volatile uint32_t *)0x40006200))
#define PC6   (*((volatile uint32_t *)0x40006100))
void ADC_Init(void){
// write this

}
int32_t ADC_In(void){
    // write this
    
  return 42;
}
// debugging dump variables
#define BUFSIZE 1000
uint32_t TimeBuf[BUFSIZE]; // in bus cycles
uint32_t DataBuf[BUFSIZE]; // 0 to 4095 assuming constant analog input
volatile uint32_t Num;     // index from 0 to BUFSIZE-1

// time jitter variables
uint32_t MinT;    // minimum(TimeBuf[i-1] – TimeBuf[i]) for i equals 1 to BUFSIZE-1
uint32_t MaxT;    // maximum(TimeBuf[i-1] – TimeBuf[i]) for i equals 1 to BUFSIZE-1
uint32_t Jitter;  // MaxT – MinT (in bus cycles)
uint16_t Periods[256];  // histogram of times between ADC triggers, optional

// SNR variables
uint32_t Averaging; // 1,2,4,8,16,32, or 64 to student CLT
uint32_t Vmin, Vmax, PMFmax;
int32_t Signal,Noise,SNR,Distance;
uint16_t PMF[100];  // histogram of ADC samples

// Samples ADC at 1000 Hz
void RealTimeTask(void){uint32_t ADCvalue;
  GPIO_PORTC_DATA_R ^= 0x80;  // profile
  GPIO_PORTC_DATA_R ^= 0x80;  // profile
  ADCvalue = ADC_In();
  if(Num < BUFSIZE){
    TimeBuf[Num] = TIMER1_TAR_R;
    DataBuf[Num] = ADCvalue;
    Num++;
  }
  GPIO_PORTC_DATA_R ^= 0x80;  // profile
}
void CalculateJitter(void){
// MaxT = maximum(Time[i-1] – Time[i]) for i equals 1 to 999
// MinT = minimum(Time[i-1] – Time[i]) for i equals 1 to 999
// Jitter = MaxT – MinT 
// write this

}
uint32_t sqrt2(uint32_t s){ int n; // loop counter
uint32_t t;            // t*t will become s
  t = s/16+1;          // initial guess
  for(n = 16; n; --n){ // will finish
    t = ((t*t+s)/t)/2;
  }
  return t;
}

void CalculateSNR(void){
  // signal is the average of BUFSIZE ADC samples
  // noise is the standard deviation of the BUFSIZE ADC samples
  // SNR = signal/noisem(no floating point allowed)
    // write this
    
}
void CreatePMF(void){
  int i;
  Vmin = 0xFFFFFFFF; Vmax = 0;
  for(i=0; i<BUFSIZE; i++){uint32_t v;
    v = DataBuf[i];
    if(v>Vmax) Vmax = v;
    if(v<Vmin) Vmin = v;
  }    
  for(i=0; i<100; i++){
    PMF[i] = 0;
  }    
  if(Vmin>25) Vmin = Vmin-25;   // room     
  PMFmax = 0;       
  for(i=0; i<BUFSIZE; i++) {
    uint32_t v;
    v = DataBuf[i];
    if((v>=Vmin)&&(v<(Vmin+100))){
      PMF[v-Vmin]++;
      if(PMF[v-Vmin]>PMFmax) PMFmax =PMF[v-Vmin];
    }        
  }
  UART_OutString("Averaging =");UART_OutUDec(Averaging); UART_OutChar(CR); UART_OutChar(LF);
  for(i=0; i<100; i++){
    UART_OutUDec(Vmin+i); UART_OutChar(',');UART_OutUDec(PMF[i]); 
    UART_OutChar(CR); UART_OutChar(LF);
  }
}
  
// initialize PC6 and PC7 as GPIO outputs
// warning: this code must be friendly because the debugger is on PC3-0
void PortC_Init(void){
  SYSCTL_RCGCGPIO_R |= 0x00000004;     // activate clock for Port C
  while((SYSCTL_PRGPIO_R&0x04) == 0){};// allow time for clock to stabilize
  GPIO_PORTC_PCTL_R = GPIO_PORTC_PCTL_R&0x00FFFFFF;  // PC6-7 GPIO
  GPIO_PORTC_DIR_R |= 0xC0;            // PC6-7 out
  GPIO_PORTC_DEN_R |= 0xC0;            // enable digital I/O on PC6-7
  GPIO_PORTC_DATA_R &= ~0xC0;          // off
}

// -----------          Timer1_Init           ---------------------------
// Reading TIMER1_TAR_R will return the 32-bit current time in 12.5ns units. 
// The timer counts down. 
void Timer1_Init(void){
  volatile uint32_t delay;
  SYSCTL_RCGCTIMER_R    |= 0x02;                // 0) activate TIMER1a
  delay                  = SYSCTL_RCGCTIMER_R;  // allow time to finish activating
  TIMER1_CTL_R           = 0x00000000;          // 1) disable TIMER1A during setup
  TIMER1_CFG_R           = 0x00000000;          // 2) configure for 32-bit mode
  TIMER1_TAMR_R          = 0x00000002;          // 3) configure for periodic mode, default down-count settings
  TIMER1_TAILR_R         = 0xFFFFFFFF;          // 4) reload value
  TIMER1_TAPR_R          = 0;                   // 5) bus clock resolution
  TIMER1_CTL_R           = 0x00000001;          // 10) enable TIMER1A
}
// d = A/(ADCvalue+B);
// short range IR GP2Y0A41SK0F
//#define A 264336
//#define B -175
//#define IRmax 2552
//#define Dmax 400
// long range IR GP2Y0A21YK0F
// ***Calibrate your sensor, DO NOT USE THESE VALUES***
#define A 293152
#define B -214
#define IRmax 900
#define Dmax 400
int32_t IR_Convert(int32_t n){  // returns distance in mm
    // write this
  
    return 42;
}
int32_t InvokesDivide=10000; 
// The purpose of InvokesDivide is to invoke a divide instruction
// Otherwise, InvokesDivide has no functional purpose

// ----------------------------------------------------------------------
// -------------------    MAIN without SSD1306  ----------------------------------------
// ----------------------------------------------------------------------
// Use debugger to observe results
// PMF output to serial port
int main(void){ 
  DisableInterrupts(); 
  PLL_Init(Bus80MHz);                // 80 MHz
  LaunchPad_Init();
  Timer1_Init();
  PortC_Init();
  UART_Init();  // to print the PMF
  Timer2A_Init(&RealTimeTask,80000,1); // 1kHz, priority=1
  ADC_Init();           // sample PE5
  EnableInterrupts(); 
  while(1){
    ADC0_SAC_R = 0; // this will hard fault if ADC_Init is not complete
    for(Averaging = 1; Averaging<=64; ){
      Num = 0;  // empty array
      while(Num<1000){
        InvokesDivide = (InvokesDivide*12345678)/12345; // this line should have caused jitter
        GPIO_PORTC_DATA_R ^= 0x40;    // toggles PC6 when running in main, this line has a critical section
      }
      CalculateJitter();
      CalculateSNR();
      CreatePMF();
      Distance = IR_Convert(Signal);

      if(PF4==0x00){                 // change SAR size on button push
        Clock_Delay1ms(10);          // debounce
        while(PF4==0x00){
          Clock_Delay1ms(10);
        }
        Averaging = Averaging<<1;
        ADC0_SAC_R = ADC0_SAC_R+1;

      } 
    }
  }
}

// ----------------------------------------------------------------------
// -------------------    MAIN with SSD1306  ----------------------------------------
// ----------------------------------------------------------------------
// PMF output to serial port
int main1(void){ 
  DisableInterrupts(); 
  PLL_Init(Bus80MHz);                // 80 MHz
  LaunchPad_Init();
  SSD1306_Init(SSD1306_SWITCHCAPVCC);
  SSD1306_ClearBuffer();
  SSD1306_OutBuffer();
  SSD1306_SetCursor(0,0);
  SSD1306_OutString("Lab 2");
  SSD1306_SetCursor(0,1);
  SSD1306_OutString("SAC =");

  SSD1306_SetCursor(0,2);
  SSD1306_OutString("ADC =");
  SSD1306_SetCursor(0,3);
  SSD1306_OutString("Dis =");
  Timer1_Init();
  PortC_Init();
  UART_Init();  // to print the PMF
  Timer2A_Init(&RealTimeTask,80000,1); // 1kHz, priority=1
  ADC_Init();           // sample PE5
  SSD1306_SetCursor(6,1);
  SSD1306_OutUDec(ADC0_SAC_R);
  EnableInterrupts(); 
  while(1){
    ADC0_SAC_R = 0; 
    for(Averaging = 1; Averaging<=64; ){
      Num = 0;  // empty array
      while(Num<1000){
        InvokesDivide = (InvokesDivide*12345678)/12345; // this line should have caused jitter
        GPIO_PORTC_DATA_R ^= 0x40;    // toggles PC6 when running in main, this line has a critical section
      }
      CalculateJitter();
      CalculateSNR();
      CreatePMF();
      Distance = IR_Convert(Signal);
      SSD1306_SetCursor(6,2);
      SSD1306_OutUDec(Signal);
      SSD1306_SetCursor(6,3);
      SSD1306_OutUDec(Distance);

      if(PF4==0x00){                 // change SAR size on button push
        Clock_Delay1ms(10);          // debounce
        while(PF4==0x00){
          Clock_Delay1ms(10);
        }
        Averaging = Averaging<<1;
        ADC0_SAC_R = ADC0_SAC_R+1;
        SSD1306_SetCursor(6,1);
        SSD1306_OutUDec(ADC0_SAC_R);

      } 
    }
  }
}
