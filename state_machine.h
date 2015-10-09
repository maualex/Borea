
typedef enum
{
    SYSTEM_BOOT = 0,
    INIT,
    CALIBRATION,
    READY,
    CONTROL
} SYSTEM_STATE;

//variabili di ingresso alla libreria...non vanno qui?!
extern unsigned char flag_calibration;   
extern unsigned char flag_control;
extern unsigned char flag_control_init;
extern unsigned char flag_calibration_request;
extern unsigned char flag_actuator;	//definito in 'propeller.h'

void state_start(void);         //unico prototipo quindi unica funzione accessibile dall'esterno

extern void (*stateS)(void);    //dichiarazione globale di puntatore a funzione

extern SYSTEM_STATE machine_state;      //variabile globale -> deve invece essere privata cioè protetta 
                                        //altrimenti chi vuole può modificare lo stato della macchina! anche non volendo.
