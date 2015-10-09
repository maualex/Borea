//#ifndef __33FJ256GP710A_H
//#include <p33FJ256GP710A.h>
//#endif
//#include "propellers.h"
//#include "quadcopter_borea.h"
//#include "spi.h"
//#include "config.h"
//#include "receiver.h"
//
//extern unsigned char machine_state;
//
//extern unsigned char actuator_timeout;
//extern unsigned int timeout_switch_on;
//extern unsigned char controllo_off;



//
//void module_config(void) {
//  
//    
////----------------------------------
//// CONFIGURAZIONE TIMER 
//    
//    /* TIMER1 configuration for 50Hz (MAIN TIMER) */
//	T1CONbits.TON   = 0;    	// timer off
//	T1CONbits.TSIDL = 0;    	// timer in funzione anche se il sistema è nello stato Idle
//	T1CONbits.TGATE = 0;    	// modalità "gated timer" disabilitata
//	T1CONbits.TCKPS = 0b10; 	// prescaler 1:64 -> Tck = 64/40MHz
//	T1CONbits.TSYNC = 0b00; 	// questo campo non viene considerato perchè è solo per clock esterno
//	T1CONbits.TCS   = 0;    	// scelgo clock interno
//
//    TMR1            = 0x00;     // azzero il timer
//    PR1             = 12500;    // 50Hz -> T = PR1*Tck = 12500*1.6us = 20ms
//    
//    _T1IP   		= 7;        // interrupt timer priority
//    _T1IE   		= 1;        // abilito interrupt timer
//    _T1IF   		= 0;	    // cancello flag
//     
//
//    /* TIMER2 configuration for 400Hz (PWM TIMER) */
//	T2CONbits.TON   = 0;    	// timer off
//	T2CONbits.TSIDL = 0;    	// timer in funzione anche se il sistema è nello stato Idle
//	T2CONbits.TGATE = 0;    	// modalità "gated timer" disabilitata
//	T2CONbits.TCKPS = 0b01; 	// prescaler 1:8 -> Tck = 8/40MHz = 0.2us
//	T2CONbits.TCS   = 0;    	// scelgo clock interno
//    
//    TMR2 			= 0x00; 	// azzero il timer
//    PR2  			= 12500;	// 400Hz -> T = PR2*Tck = 12500*0.2us = 2.5ms 
//    
//    _T2IP 			= 5;      	// interrupt timer priority 
//    _T2IE 			= 1;      	// abilito interrupt timer
//    _T2IF 			= 0;      	// cancello flag
//
//    
//    /* TIMER5 configuration for 50Hz (WAIT TIMER) */
//	T5CONbits.TON   = 0;    	// timer off
//	T5CONbits.TSIDL = 0;    	// timer in funzione anche se il sistema è nello stato Idle
//	T5CONbits.TGATE = 0;    	// modalità "gated timer" disabilitata
//	T5CONbits.TCKPS = 0b10; 	// prescaler 1:64 -> Tck = 64/40MHz
//	T5CONbits.TCS   = 0;    	// scelgo clock interno
//    
//    TMR5 			= 0x00; 	// azzero il timer
//    PR5             = 12500;    // 50Hz -> T = PR1*Tck = 12500*1.6us = 20ms 
//    
//    _T5IP 			= 1;      	// interrupt timer priority 
//    _T5IE 			= 0;      	// abilito interrupt timer
//    _T5IF 			= 0;      	// cancello flag
//    
//   
////----------------------------------
//// CONFIGURAZIONE pin 
//	
//	TRISB = 0xFFFF;           //i pin della portaB sono ingressi
//	TRISD = 0xFF;             //8 pin porta D come uscite (PWM)
//
//	//analog inputs
//	TRISBbits.TRISB15 = 1; //AN15
//	TRISCbits.TRISC1  = 1; //AN16
//	
//    
////----------------------------------
////UART1 configuration (GPS module)	
//	U1BRG = BAUD_RATE_SETTING;		// 1) BAUD Rate Setting for 4800, and Fcy = 40MHz
//	U1MODEbits.PDSEL  = 0; 			// 2) No Parity, 8-data bits	
//	U1MODEbits.STSEL  = 0; 			// 3) 1-stop bit
//	U1MODEbits.UARTEN = 0; 			// UART1 spento
//	U1STAbits.URXISEL = 0;		 	// Interrupt after one RX character is received;
//
////----------------------------------
////UART2 configuration (XBee module)
//    U2MODEbits.UARTEN   = 0;
//    U2MODEbits.USIDL    = 0;    //continue operation in idle mode
//    U2MODEbits.IREN     = 0;
//    U2MODEbits.RTSMD    = 1;    //simplex mode
//    U2MODEbits.UEN      = 0;    //only U2TX and U2RX are used
//    U2MODEbits.WAKE     = 0;    //wake-up disable
//    U2MODEbits.LPBACK   = 0;    //loopbak mode disabled
//    U2MODEbits.ABAUD    = 0;    //auto-baud rate disable
//    U2MODEbits.URXINV   = 0;    //U2RX idle state is '0'
//    
//    U2MODEbits.PDSEL    = 0b00; //8bit data, no parity
//    U2MODEbits.STSEL    = 0;    // one stop bit
//    
//    U2STAbits.UTXISEL0  = 1;   
//    U2STAbits.UTXISEL1  = 0;
//    U2STAbits.UTXINV    = 0;
//    U2STAbits.UTXBRK    = 0;  
//    U2STAbits.UTXEN     = 0;    //tramissione UART disabilitata
//    U2STAbits.URXISEL   = 0b11;    //interrupt is generated when one or two byte has been received
//    U2STAbits.ADDEN     = 0;    //address detect mode disabled
//    
//    U2MODEbits.BRGH     = 0;    //low speed mode -> detection bit more robust: 3 sample a metà bit
//    U2BRG               = 64; 
//    
//    //interrupt controller configuration
//    IEC1bits.U2RXIE     = 1;    //interrupt on recieve data enabled
//    IEC1bits.U2TXIE     = 1;    //interrupt on trasmit data disabled
//    
//    IPC7bits.U2RXIP     = 2;    //priority uart reception greater than timer 
//    IPC7bits.U2TXIP     = 2;
//    //clear flags
//    IFS1bits.U2RXIF     = 0;
//    IFS1bits.U2TXIF     = 0;
//
//
//    
////----------------------------------
//// CONFIGURAZIONE ADC1
//    AD1CON1bits.ADON = 0;     //spengo il modulo ADC1 per fare la configurazione
//
//    //setto tutti i pin come digitali
//    AD1PCFGH    = 0xFFFF;        
//    AD1PCFGL    = 0xFFFF;
//    
//    //analog pin for current connection
//    AD1PCFGHbits.PCFG16 = 0;   		//AN16
//
//    
//    //configurazione clock ADC e sample time
//    AD1CON3bits.ADRC = 0;   		//clock derivato da Tcy
//    AD1CON3bits.ADCS = 3;   		//periodo clock Tad = 4*Tcy = 300ns
//    
//    // altre configurazioni
//    AD1CON1bits.ADSIDL  = 0;         //opera anche in Idle Mode
//    AD1CON1bits.AD12B   = 1;         //conversione a 12bit
//    AD1CON1bits.FORM    = 0b00;      //formato del risultato: integer
//    AD1CON1bits.SSRC    = 0b000;     //manual start conversion with the SAMP bit
//    AD1CON1bits.ASAM    = 1;         //auto sampling
//    AD1CON1bits.SAMP	= 1;		 //sampling on turn-on
//	
//    AD1CON2bits.VCFG    = 0b000;     //tensioni di riferimento interne
//    AD1CON2bits.CSCNA   = 0;         //no modalità scanning
//    AD1CON2bits.CHPS    = 0b00;      //utilizzo solo in canale CH0 (l'unico adatto ai 12bit)
//    AD1CON2bits.BUFM    = 0;         //riempi il BUF0 iniziando sempre dal primo bit
//    AD1CON2bits.ALTS    = 0;         //uso MUX A e MUX B (channels AN15 and AN16)
//	
//	AD1CHS0bits.CH0NA   = 0;         //riferimento negativo per 'CH0 MUX B' connesso con VrefL
//	AD1CHS0bits.CH0SA   = 16;		 //riferimento positivo per 'CH0 MUX B' connesso con AN16
//	
//    //interruzioni
//    IEC0bits.AD1IE      = 0;         //interruzioni ADC disabilitate
//    IFS0bits.AD1IF      = 0;         //cancello flag interrupt
//    IPC3bits.AD1IP      = 0;   		 //interrupt priority
//    
//	
////----------------------------------
//// CONFIGURAZIONE ADC2  (BATTERY VOLTAGE) --> si potrebbe gestire con interrupt ad una frequenza lenta (5Hz?)
//    AD2CON1bits.ADON = 0;     //spengo il modulo ADC1 per fare la configurazione
//
//    
//    //analog pin for battery connection
//    AD2PCFGLbits.PCFG15 = 0;   		//AN15
//
//    //configurazione clock ADC e sample time
//    AD2CON3bits.ADRC = 0;   		//clock derivato da Tcy
//    AD2CON3bits.ADCS = 3;   		//periodo clock Tad = 4*Tcy = 300ns
//    
//    // altre configurazioni
//    AD2CON1bits.ADSIDL  = 0;         //opera anche in Idle Mode
//    AD2CON1bits.AD12B   = 1;         //conversione a 12bit
//    AD2CON1bits.FORM    = 0b00;      //formato del risultato: integer
//    AD2CON1bits.SSRC    = 0b000;     //manual start conversion with the SAMP bit
//    AD2CON1bits.ASAM    = 1;         //auto sampling
//    AD2CON1bits.SAMP	= 1;		 //sampling on turn-on
//	
//    AD2CON2bits.VCFG    = 0b000;     //tensioni di riferimento interne
//    AD2CON2bits.CSCNA   = 0;         //no modalità scanning
//    AD2CON2bits.CHPS    = 0b00;      //utilizzo solo in canale CH0 (l'unico adatto ai 12bit)
//    AD2CON2bits.BUFM    = 0;         //riempi il BUF0 iniziando sempre dal primo bit
//    AD2CON2bits.ALTS    = 0;         //only MUX A (channels AN15)
//    
//    AD2CHS0bits.CH0NA   = 0;         //riferimento negativo per 'CH0 MUX A' connesso con VrefL
//	AD2CHS0bits.CH0SA   = 15;		 //riferimento positivo per 'CH0 MUX A' connesso con AN15
//	
//    //interruzioni
//    /*
//    IEC0bits.AD2IE      = 0;         //interruzioni ADC disabilitate
//    IFS0bits.AD2IF      = 0;         //cancello flag interrupt
//    IPC3bits.AD2IP      = 0;   		 //interrupt priority
//	*/
// 
//};




