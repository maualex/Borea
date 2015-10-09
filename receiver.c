#ifndef __33FJ256GP710A_H
#include <p33FJ256GP710A.h>
#endif
#include "receiver.h"

//internal variables
volatile unsigned char rc_event[RC_CHANNELS];			// event counter 1
volatile unsigned int rc_timing[RC_CHANNELS][4];		// timing table 1
volatile unsigned char rc_data_ready_flag;				// flag for reading routine
volatile unsigned char rc_event_buf[RC_CHANNELS];		// event counter 2
volatile unsigned int  rc_timing_buf[RC_CHANNELS][4];	// timing table 2
volatile int current_cpu_ipl;
volatile unsigned char *pc;								// event pointer
volatile unsigned int  *pt;								// timing pointer
			
//global variables
volatile int rc_channel_value[RC_CHANNELS];				// output 1: duty cycle 

void rc_config()
{

	/* TIMER 3 -> frequency: 40Hz, PWM frequency around 47Hz */
    T3CONbits.TON   	= 0;    				// timer off
	T3CONbits.TSIDL 	= 0;    				// always on
	T3CONbits.TGATE 	= 0;    				// "gated timer" mode disabled
	T3CONbits.TCKPS 	= 0b10; 				// prescaler 1:64 -> Tck = 64/40MHz=1.6us
	T3CONbits.TCS   	= 0;    				// internal clock
    TMR3 				= 0x00;         		// azzero il timer
    PR3  				= RC_TIMER_PERIOD; 
    IPC2bits.T3IP 		= RC_TIMER_PRIORITY;	// interrupt timer priority 
    IEC0bits.T3IE 		= 1;      				// interrupt 
    IFS0bits.T3IF 		= 0;      				// cancello flag

    /* input pin */ 
    RC_CH_1    			= 1; 	
    RC_CH_2    			= 1; 	
    RC_CH_3   			= 1; 	
    
/**** INPUT 1 ****/ 
    IC1CONbits.ICM      = 0b000;    	//disabilito per configurare
    IC1CONbits.ICSIDL   = 0;			//opera anche con CPU in idle
    IC1CONbits.ICTMR    = 0;     		//cattura il valore del timer3 (50Hz)   
    IPC0bits.IC1IP      = RC_CHANNEL_PRIORITY;  //massima priorità
    IFS0bits.IC1IF      = 0;    		//clear interrupt flag
    IEC0bits.IC1IE      = 1;    		//enable interrupt

/**** INPUT 2 ****/    
    IC2CONbits.ICM      = 0b000;    //disable
    IC2CONbits.ICSIDL   = 0;
    IC2CONbits.ICTMR    = 0;     //cattura il valore del timer3    
    IPC1bits.IC2IP      = RC_CHANNEL_PRIORITY;    //set priority
    IFS0bits.IC2IF      = 0;    //clear interrupt flag
    IEC0bits.IC2IE      = 1;    //enable interrupt

/**** INPUT 3 ****/     
    IC3CONbits.ICM      = 0b000;    //disable
    IC3CONbits.ICSIDL   = 0;
    IC3CONbits.ICTMR    = 0;     //cattura il valore del timer3   
    IPC9bits.IC3IP      = RC_CHANNEL_PRIORITY;    //set priority
    IFS2bits.IC3IF      = 0;    //clear interrupt flag
    IEC2bits.IC3IE      = 1;    //enable interrupt
    
};

void rc_init()
{
	int channel, event;
	
	// clean data structure
	for(channel=0;channel<RC_CHANNELS;channel++)		// channels
	{
		rc_event[channel] = 0;		// reset event counter
		for(event=0;event<4;event++)			// reset timing table
		{
			rc_timing[channel][event] = 0;
		}
	}	
	
		
	// switch on timer 3
	T3CONbits.TON       = 1;			//switch on timer 3 for input capture modules
	
	
	// switch on input compare modules in "edge detection mode"
	if(RC_CH_1_ENABLED) IC1CONbits.ICM      = 0b001;    	// channel 1
	if(RC_CH_2_ENABLED) IC2CONbits.ICM      = 0b001;    	// channel 2
	if(RC_CH_3_ENABLED) IC3CONbits.ICM      = 0b001;    	// channel 3
	
};

void __attribute__((__interrupt__,__no_auto_psv__)) _IC1Interrupt(void) //spinta verticale "input 1"
{
	// REMARK: in questo punto TMR3 e IC1BUF dovrebbero essere circa uguali
	// NOTA: se il nesting delle interruzioni è attivo allora la prima volta che si entra
	// in questa ISR il buffer dell'input compare è pieno quindi bisogna scaricare
	// 4 word perchè alla successiva interruzione si prelevi il valore giusto.
	// Ricorda: ad ogni lettura di IC1BUF esso si carica automaticamente con il successivo
	// valore presente nel buffer. Quando il buffer è scarico il bit ICBNE del registro di
	// controllo si abbassa. Un ulteriore lettura restituirà il valore presente nel registro
	// di scrittura IC1BUF (4 word nel buffer + 1 word registro). Ulteriori letture
	// restituiranno sempre il medesimo valore.
	
	// REMARK: 	solo nella prima interruzione il buffer è pieno di valori non validi e solo
	//			se il nesting delle interruzioni è attivo.
	// REMARK: 	ad ogni interruzione non è detto che ci sia stato un unico evento (edge detection).
	//			questo perchè dipende da quanto temop si impiega a servire questa ISR. 
	//			Il flag dell'interruzione viene settato ad evento rilevato ma la ISR
	//			potrebbe essere richiamata dopo varie volte che tale flag sia stato settato.
	//			questo perchè pezzi di codice con priorità 7 possono consumare molto tempo.
	//			In tali circostanze infatti il contatore degli eventi segnava 1, cosa non possibile,
	//			infatti un evento veniva perso.
	//			Gli eventi possono essere recuperati se si sfrutta il buffer del modulo ed è
	//			ciò che viene realizzato qui. Pertanto la perdita di eventi è assai più ridotta.
	
    _IC1IF 	= 0;		
    
    //SET_AND_SAVE_CPU_IPL(current_cpu_ipl,7); 	//questa istruzione è pericolosa.
	//_IC2IE = 0;	//<-se blocca qui la IC3 allora IC2IE torna ad 1! non va bene questa soluzione
	//_IC3IE = 0;	//bisognerebbe usare una variabile condivisa per fare lock() e unlock()
					//NOTA: qui la memoria in effetti non è condivisa quindi non cè bisogno
			// shared memory pointers
			pt 	= (unsigned int*)&rc_timing[RC_1][0];
			pc 	= (unsigned char*)&rc_event[RC_1];
	//_IC2IE = 1;
	//_IC3IE = 1;		
			while(IC1CONbits.ICBNE)			// leggo tutto il buffer
			{
				if(*(pc)<4) 				// la condizione serve a non superare il limite dell'array
				{
					*(pt + *(pc)) 	= IC1BUF;
				}
				*(pc) = *(pc) + 1;			// il conteggio può comunque superare la dimensione dell'array -> serve per	
			}								// l'identificazione degli errori nella routine 'rc_in_duty_cycle()'
	
	//RESTORE_CPU_IPL(current_cpu_ipl); //questa istruzione è pericolosa. può non tornare la priorità di prima
	
};


void __attribute__((__interrupt__,__no_auto_psv__)) _IC2Interrupt(void) 
{
    _IC2IF 	= 0;

	pt 		= (unsigned int*)&rc_timing[RC_2][0];
	pc 		= (unsigned char*)&rc_event[RC_2];

	while(IC2CONbits.ICBNE)	
	{
		if(*(pc)<4) 			
		{
			*(pt + *(pc)) 	= IC2BUF;
		}
		*(pc) = *(pc) + 1;	
	}
	 
};


void __attribute__((__interrupt__,__no_auto_psv__)) _IC3Interrupt(void) 
{
    _IC3IF 	= 0;
    
	pt 		= (unsigned int*)&rc_timing[RC_3][0];
	pc 		= (unsigned char*)&rc_event[RC_3];

	while(IC3CONbits.ICBNE)	
	{
		if(*(pc)<4) 			
		{
			*(pt + *(pc)) 	= IC3BUF;
		}
		*(pc) = *(pc) + 1;	
	}	
};

 
void __attribute__((__interrupt__,__no_auto_psv__)) _T3Interrupt(void) // deve partire prima degli input capture
{
	
	IFS0bits.T3IF = 0;			//NOTA: mettere un break point qui fa entrare due volte -> breakpoint alla fine
	// local variables
	unsigned int i;
	unsigned int *p_in, *p_out;

	SET_AND_SAVE_CPU_IPL(current_cpu_ipl,7);			// REGIONE CRITICA -> ACCESSO ALLA MEMORIA CONDIVISA
 
		p_in  = (unsigned int*)&rc_timing[0][0]; 		// table base address 1
		p_out = (unsigned int*)&rc_timing_buf[0][0]; 	// table base address 2
		
		for(i=0;i<(4*RC_CHANNELS);i++)		// buffer copy
		{
			*(p_out+i) = *(p_in+i);
			*(p_in+i)  = 0;					// clear buffer (unnecessary?)
		}
		for(i=0;i<RC_CHANNELS;i++)			// buffer copy
		{
			rc_event_buf[i] = rc_event[i];
			rc_event[i] 	= 0;			// clear event counter
		}
		rc_data_ready_flag = 1;				// flag for the reading routine 'rc_in_duty_cycle()'
		
	RESTORE_CPU_IPL(current_cpu_ipl);

};


void rc_in_duty_cycle(unsigned char* error)
{
	int dt1, dt2, rc_p, i; 

	//nota: nel primo buffer 'rc_timing' gli output compare scrivono in modo asincrono
	//      mentre nel secondo buffer 'rc_timing_buf' la scrittura avviene in modo sincrono
	//		con il timer 3. In questo modo il secondo buffer è sempre pieno evitando così
	//		letture parziali.
	// il timer3 è il processo produttore mentre questa funzione rappresenta il consumatore
	// il consumatore può sempre consumare, quindi può prelevare anche lo stesso dato.
	// ciò accade sicuramente perchè il produttore scrive a 40Hz mentre il consumatore legge a 50Hz.
	// la variabile di uscita è il vettore 'rc_channel_value[]'.
	
	if(rc_data_ready_flag)		// se il dato è pronto (timer 3 isr) allora ricalcolo il duty cycle
	{		
		for(i=0;i<RC_CHANNELS;i++)
		{
			SET_AND_SAVE_CPU_IPL(current_cpu_ipl,7);	//inizio zona critica
				dt1 		= rc_timing_buf[i][1] - rc_timing_buf[i][0];
				dt2 		= rc_timing_buf[i][2] - rc_timing_buf[i][1];
				rc_p 		= rc_event_buf[i];
			RESTORE_CPU_IPL(current_cpu_ipl);			//fine zona critica
			error[i] 	= 0;
			
			// timing corrections
			if(dt1<0)
			{
				dt1	+= RECEIVER_MAX;
			}
			if(dt2<0)
			{
				dt2	+= RECEIVER_MAX;
			}	
				
			if(rc_p<2)						// events detected: less than 2 -> impossible
			{
				error[i] = 1;	
				// the previous duty cycle remains valid
			}
			else if(rc_p==2)				// events detected: 2
			{
				rc_channel_value[i]  	= dt1;     
			}
			else if(rc_p==3 || rc_p==4)		// events detected: 3 or 4
			{
				// nota: considero sempre solo i primi 3 eventi (per semplificare l'algoritmo)
				//la soluzione corretta è l'intertempo più piccolo
				if(dt1<dt2)
				{
					rc_channel_value[i]  	= dt1;
				}
				if(dt1>dt2)
				{
					rc_channel_value[i]  	= dt2;
				}
				if(dt1==dt2)
				{
					error[i] = 2;
				}
			}
			else
			{
				error[i] = 3;		//more than 4 event have been detected -> isr waiting to much
			}
		}
		rc_data_ready_flag = 0;
	} // end data ready	
};

float rc_ch_conditioning(int x, int ch0)
{
		int dx1, dx2, dx;
		float rc_ch;
		
		dx1 = ch0 - RC_SAT_MIN;
		dx2 = RC_SAT_MAX - ch0;
		
		//l'intervallo simmetrico [-dx,+dx] sarà dato dalla corsa più corta
		if(dx1<dx2) dx = dx1;
		else dx = dx2;
		
		//comando normalizzato
		rc_ch = (float)(x-ch0)/dx;
		//saturazioni
		if((x-ch0)<-dx) rc_ch = - 1;
		if((x-ch0)>dx) rc_ch =  1;
		return rc_ch;
};
