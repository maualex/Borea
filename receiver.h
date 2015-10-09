/********************************************************************************************************************/
/* Autore				: Mauricio Alejandro Lotufo																	*/
/* Data					: 07/05/2015																				*/
/* Descrizione			: legge a 40Hz il comando ricevuto (duty cycle) da un telecomando che rispetta 				*/				
/*						  lo standard servo con frequenza di circa 50Hz.											*/
/* Risorse utilizzate	: Timer 3, moduli 'input capture'															*/																						
/********************************************************************************************************************/
/*
  	nota 1	: per 'evento' qui si intende il rilevamento di un fronte di salita o di discesa del 
			: segnale PWM generato dalla ricevente.
	nota 2	: il periodo del segnale PWM non è molto stabile (frequenza circa 47Hz).
	nota 3  : il segnale PWM rispetta lo standard servo ovvero segnale alto compreso tra 1ms e 2ms.
	
 	Problem	: misurare il duty cycle del segnale:
 			: -> condizione necessaria: 2 eventi
 			: -> condizione necessaria e sufficiente: 3 eventi
 	
	Solution: sia Tp il periodo del segnale PWM di cui si vuole rilevare il duty cycle e 
			: sia Tc il periodo del timer impiegato per la misura dei tempi.
			: caso Tc=Tp -> (condizione non verificabile) potrebbero essere rilevati 0, 1, 2 eventi e solo
			: il caso con 2 eventi sarebbe utile al calcolo del duty cycle.
			: Perchè ci siano sempre almeno 2 eventi deve valere Tc=Tp+dT ma esiste un caso dove i due eventi
			: non sono sufficienti (primo evento fronte di discesa e secondo evento fronte di salita).
			: Per evitare questa circostanza e quindi il mancato aggiornamento del duty cycle, deve valere 
			: dT>max(tempo segnale alto)=2ms. Con questa modifica gli eventi possono arrivare a 4.
			: Si sceglie quindi un periodo Tc>Tp+2ms. Gli eventi possibili vanno da un minimo di 2 a un massimo 
			: di 4 pertanto il duty cycle è sempre correttamente rilevabile ad ogni Tc.
			: essendo Tp circa 22ms si è scelto Tc = 25ms. Ciò comporta una frequenza di aggiornamento del 
			: duty cycle pari a 40Hz.
	
	nota 4	: Il tempo che intercorre tra una interruzione (timer che raggiunge il valore massimo) e il servizio 
			: dell'interruzione stessa dipende da quanto codice a più alta priorità ne blocca il servizio.
			
	nota 5	: Dalla (nota 4) ne deriva che nel caso qui presente la ISR TIMER 3 può essere richiamata dopo un tempo 
			: superiore a Tc e quindi gli eventi rilevati fino a quel momento possono superare i 4 previsti.
			: In tale evenienza il codice garantisce il funzionamento corretto perchè gli ulteriori eventi vengono
			: sovrascritti e non considerati (verificare meglio questa condizione). 
			
	nota 6 	: Per la (nota 4) anche le ISR dei moduli 'input capture' possono essere ritardate rispetto all'arrivo
			: dell'interruzione. L'hardware riesce comunque a salvare i nuovi eventi grazie al buffer di cui questi
			: moduli sono dotati. La loro dimensione è però limitata a 4 eventi. Un ritardo maggiore comporta la perdita 
			: di eventi. Riassumendo ritardare il servizio comporta la presenta di più eventi presenti nel buffer. 
			: Questa condizione è supportata da questa libreria prelevando ad ogni ISR tutti gli eventi presenti nel buffer.

*/
/********************************************************************************************************************/

#define RECEIVER_MAX 		15625
#define RC_CHANNELS 			3
#define RC_CHANNEL_PRIORITY 	6
#define RC_TIMER_PRIORITY 		5	//non può interrompere i canali
#define RC_TIMER_PERIOD		15625	// T=PR1*Tck=15625*1.6us=25ms -> 40Hz
#define RC_CH_1_ENABLED 1
#define RC_CH_2_ENABLED 1
#define RC_CH_3_ENABLED 1

//channel pin
#define RC_CH_1 		TRISDbits.TRISD8	//input OC1 - stick sinistro:thrust
#define RC_CH_2 		TRISDbits.TRISD9	//input OC2 - stick destro: destra-sinistra
#define RC_CH_3 		TRISDbits.TRISD10	//input OC3 - stick destro: avanti-indietro

#define RC_1 			0		//left stick : up % down	(total thrust command)
#define RC_2 			1		//right stick: left & right	(roll command)
#define RC_3 			2		//right stick: up % down 	(pitch command)

//______________________COMMON PROPERTIES______________________
//saturation
#define RC_SAT_MIN 		625
#define RC_SAT_MAX 		1250


//centering
#define RC_STICK_CENTERING 0.001

//__________________________CHANNEL_01__________________________
//offset
#define RC_CH1_MIN      697 		//valore minimo
//offset
#define RC_CH1_MAX     1200 		//valore massimo
//scale factor
#define RC_1_GAIN       20 			// 20N max (+12N = 32N -> 8N per motore)
//filter
#define RC_1_BETA       0.7
//valore minimo della spinta verticale (perchè lo zero reale è maggiore di 625)
#define RC_1_MIN_THRUST 1           //[N]
#define RC_CH1_NORMALIZE 	0.00198		// 1/(RC_SAT_MAX - RC_ZERO_CH1)

//__________________________CHANNEL_02__________________________
//offset
#define RC_ZERO_CH2 949 			//valore centrale
//scale factor
#define RC_2_GAIN 0.17				// maximum angle (10°)
//filter
#define RC_2_BETA 0.2

//__________________________CHANNEL_03__________________________
//offset
#define RC_ZERO_CH3 931 			//valore centrale
//scale factor
#define RC_3_GAIN 0.17				// maximum angle (10°)
//filter
#define RC_3_BETA 0.2


//global variables
extern volatile int rc_channel_value[RC_CHANNELS];	// commands received
// prototypes
void rc_config(void);
void rc_init(void);
void rc_in_duty_cycle(unsigned char*);
void rc_data_process(void);
float rc_ch_conditioning(int x, int ch0);
