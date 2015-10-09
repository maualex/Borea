/**** ADC INIT ****/

#include <p33FJ256GP710A.h>
#include "MPU6000_lib.h"
#include "ADC_lib.h"

int ADCValue_buffer;
int i,n;
int buffer_switch=0;

extern float V_battery[10];
extern float Sonar_m[10];


int ADC_Init(void)
{

int k;
for(k=0;k<10;k++)V_battery[k]=0; // buffer init.



AD1CON1bits.ADON =0 ; 			// Disable ADC
_AD1IE = 0; 					// Interrupt disabled



	/*** ADC configuration ***/

//1)
		AD1CON1bits.AD12B 	=   0;	    // 10 bits instead of 12

//2)
		AD1CON2bits.VCFG 	=   7; 	    // Converter voltage reference 0b111 

//3)
		AD1CON3bits.ADRC 	=   0; 		// CLOCK SOURCE IS THE INTERNAL RC CLOCK. 0 FOR USING SYSTEM CLOCK
		AD1CON3bits.ADCS 	=  50; 		// (ADCS VALUE IN DEC +1 )*TCY = TAD = 1.275 uS with FCY = 40MHz

//4)
		TRISAbits.TRISA15 	=   1; 		// analogue pin set as input

//5)
		AD1CON2bits.CHPS  	=  	0;		// converts CH0 only
		AD1CHS0bits.CH0NA 	=  	0; 		// NEGATIVE INPUT IS VREFL
		AD1CHS0bits.CH0SA 	=  15; 		// pin A15 (battery)
		AD1CHS0bits.CH0NB 	=  	0; 		// NEGATIVE INPUT IS VREFL
		AD1CHS0bits.CH0SB 	=  16; 		// pin A16 (sonar)

//6)
		AD1PCFGLbits.PCFG15 = 0; 		// Pin 15 in analogue mode
		AD1PCFGHbits.PCFG16 = 0; 		// Pin 16 in analogue mode
//7)
		AD1CON1bits.SIMSAM	= 0;		// Simultaneous Sample Select bit, disabled (can't use it, we get just one channel, CH0)
		AD1CSSLbits.CSS15 	= 1;      	// SCAN THE CHOSEN INPUTS, irrelevant if AD1CON2bits.ALTS  = 0; 
		AD1CSSHbits.CSS16 	= 1;
//8)
		AD1CON1bits.ASAM 	= 1;		// If 1, AUTOSAMPLING,SAMP bit is auto-set. In this case SAMP is set manually

//9)
		AD1CON3bits.SAMC    =30;     		// SAMPLING TIME = VALUE *TAD IF SSRC = 7		

//10)
		AD1CON1bits.FORM =    0;		// Integer (DOUT = 0000 00dd dddd dddd)

//11)
		AD1CON2bits.CSCNA   = 1;        // 1 Scan inputs for CH0+ during sample A bit, 0 do not scan CHANGED
		AD1CON1bits.SSRC 	= 7; 		// Auto conversion (See the notes at the end of the code)
		AD1CON2bits.BUFM  	= 0; 		// Always starts filling the buffer from the start address.
		AD1CON2bits.BUFS 	= 0; 		// Disabled, it works only if BUFM =1
		AD1CON2bits.ALTS  	= 0; 		// Alternate input scan disabled
		AD1CON2bits.SMPI  	= 1;		// GENERATE INTERRUT EVERY 2 SAMPLE CONVERSIONS

//12)
	    AD1CON4bits.DMABL = 0;	// Selects Number of DMA Buffer Locations per analogue Input bits: Allocates 1 word of buffer to each analogue input 
		//DMA not needed
//13)
		//data format chosen yet

//14)
		_AD1IF = 0; // Interrupt flag cleared
  		_AD1IP = 4; // Priority
  	    _AD1IE = 1; // Interrupt enabled

//15)
		//configure DMA CHANNEL if needed

//16)
		AD1CON1bits.ADON =1 ;      // ADC Enabled


/*The DONE bit (AD1CON1<0>) is set when an ADC interrupt is generated to indicate completion
of a required sample/conversion sequence*/


return 0;

};


void ADC_read(void)
{   
  
	AD1CON1bits.SAMP = 1; 			// Start sampling
	while (!AD1CON1bits.DONE);		// Conversion done? (this causes the interrupt)
	
};

void __attribute__ ((__interrupt__, __auto_psv__)) _ADC1Interrupt(void)   
{   
	_AD1IF = 0; 							// Interrupt flag cleared

	
	if (buffer_switch %2 == 0) 					// even number
	{

 
		ADCValue_buffer = ADC1BUF0; 
		Sonar_m[n] = ADCValue_buffer;
		n++;
		buffer_switch++;
	}

		else 								// odd number
	{
 	ADCValue_buffer = ADC1BUF0; 			// Get ADC value
	V_battery[i] = ADCValue_buffer;			// Put it in the buffer
	i++;									// Increase counter to fill all the five positions of the buffer
	buffer_switch++;
	}


	if(i==10) i=0;							// After five iterations restart from the first position and so on
    if(n==10) n=0;

};

