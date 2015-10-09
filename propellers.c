#include "propellers.h"
#include <p33FJ256GP710A.h>

void prop_set(const unsigned int* duty)  
{
		ESC_1 = duty[P1];
	    ESC_2 = duty[P2];
	    ESC_3 = duty[P3];
	    ESC_4 = duty[P4];  
};

void prop_off()
{           
    ESC_1 = UD_OFFSET;        
    ESC_2 = UD_OFFSET;         
    ESC_3 = UD_OFFSET;    
    ESC_4 = UD_OFFSET;  
};

void prop_config(void)
{
// motore 1 
    OC2CONbits.OCM    = 0b000;  	//modulo spento
    OC2R              = UD_OFFSET; 	//dutycicle=0 per il primo ciclo
    OC2RS             = UD_OFFSET; 	//dutycicle=0 per il secondo ciclo
    OC2CONbits.OCSIDL = 0;      	//funziona anche in idle mode della cpu
    OC2CONbits.OCTSEL = 0;      	//sorgente clock: timer2
    
// motore 2 
    OC3CONbits.OCM    = 0b000;  	//modulo spento
    OC3R              = UD_OFFSET; 	//dutycicle=0 per il primo ciclo
    OC3RS             = UD_OFFSET; 	//dutycicle=0 per il secondo ciclo
    OC3CONbits.OCSIDL = 0;      	//funziona anche in idle mode della cpu
    OC3CONbits.OCTSEL = 0;      	//sorgente clock: timer2
    
// motore 3 
    OC4CONbits.OCM    = 0b000;  	//modulo spento
    OC4R              = UD_OFFSET; 	//dutycicle=0 per il primo ciclo
    OC4RS             = UD_OFFSET; 	//dutycicle=0 per il secondo ciclo
    OC4CONbits.OCSIDL = 0;      	//funziona anche in idle mode della cpu
    OC4CONbits.OCTSEL = 0;      	//sorgente clock: timer2
    
// motore 4 
    OC5CONbits.OCM    = 0b000;  	//modulo spento
    OC5R              = UD_OFFSET; 	//dutycicle=0 per il primo ciclo
    OC5RS             = UD_OFFSET;  //dutycicle=0 per il secondo ciclo
    OC5CONbits.OCSIDL = 0;      	//funziona anche in idle mode della cpu
    OC5CONbits.OCTSEL = 0;      	//sorgente clock: timer2
    
};

void prop_start(void)
{
	int i;
	long int j;
	
	OC2CONbits.OCM    = 0b110;  	//avvio modulo in PWM mode senza fault protection
	OC3CONbits.OCM    = 0b110;  	//avvio modulo in PWM mode senza fault protection
	OC4CONbits.OCM    = 0b110;  	//avvio modulo in PWM mode senza fault protection
	OC5CONbits.OCM    = 0b110;  	//avvio modulo in PWM mode senza fault protection

	//wait for ESC activation
    for(i=0;i<ESC_WAIT;i++)
    {
        for(j=0;j<10000;j++)   //1ms circa
        {
            Nop();
        }
    }
};
