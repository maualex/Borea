#include <p33FJ256GP710A.h>

#ifndef __33FJ256GP710A_H
#include <p33FJ256GP710A.h>
#endif


#define ADC_conversion 0.00322265625  // From digital to voltage
#define ADC_factor_INV 4.41700341325  // Line's angular coefficient for battery CHANGE THE NAME MATE!
#define ADC_Bias 0.0573				  // BATTERY CALIBRATION 	
#define Diode_V_Drop 0.6			  // Silicon diod voltage drop (1N4002)
#define SONAR_factor 6.2060606060606060606060606060606	  // 1/(3.3/(1024*0.02))

int ADC_Init(void);
void ADC_read(void);


