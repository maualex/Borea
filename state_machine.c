#include "system.h"
#include "state_machine.h"


//possibili miglioramenti per questa libreria:
//1. I flag creano delle dipendenze cioè la libreria state machine dipende da questi flag che rappesentano ingressi
//2. La variabile che rappresenta lo stato deve essere protetta ovvero interna solo a questo modulo
//   a tale scopo la si dichiara statica e si fornisce un funzione 'getstate()' per accedervi in lettura dall'esterno.

extern unsigned char flag_control_init, flag_calibration, flag_control, flag_calibration_request;

SYSTEM_STATE machine_state;

int led_toggle;

//private function
void ent_init_cu(void);
void ent_calibration(void);
void ent_control_on(void);
void ent_ready(void);

//puntatore a funzione (globale, chi include il .h lo avrà a disposizione perchè dichiarato extern nel .h)
void (*stateS)(void);

void state_start()
{
	led_toggle 	= 0;
    //LED_RED 	= LED_OFF;
	//LED_GREEN 	= LED_ON;
	ent_init_cu();
};

void state_initialization()
{
    //LED_RED 	= LED_OFF;
	if(flag_control_init==OFF && flag_calibration_request==ON) ent_calibration();
    //ent_calibration();
};

void state_calibration()
{
	if(led_toggle<32000) led_toggle++;
	else led_toggle = 0;
	if(led_toggle%10==0) LED_BLUE = ~LED_BLUE;
	
	if(flag_calibration==OFF) ent_ready();			//when calibration is finished then you are ready to go
};

void state_ready()	//ready to switch-on the control loop
{
	if(led_toggle<32000) led_toggle++;
	else led_toggle = 0;
	if(led_toggle%25==0) LED_GREEN = ~LED_GREEN;
	
	if(flag_calibration_request==ON) ent_calibration();	//it possible to perform calibration again
	if(flag_actuator==ON) ent_control_on();

    //ent_control_on();
};

void state_control_on()
{
	if(flag_actuator==OFF) ent_init_cu();
};

void ent_init_cu()
{
	machine_state		= INIT;
	flag_control_init 	= ON;
	flag_control		= OFF;
	stateS 				= &state_initialization;
	//led coding state
	LED_RED 			= LED_OFF;
	LED_BLUE 			= LED_OFF;
	LED_GREEN 			= LED_OFF;
};

void ent_calibration()
{
	machine_state				= CALIBRATION;
	flag_calibration   			= ON;
	flag_calibration_request 	= OFF;
	stateS 						= &state_calibration;
	//led coding state
	LED_BLUE 			= LED_ON;
	LED_RED 			= LED_OFF;
	LED_GREEN 			= LED_ON;
};

void ent_control_on()
{
	machine_state		= CONTROL;
	flag_control		= ON;	
	stateS 				= &state_control_on;
	//led coding state
	LED_BLUE 			= LED_ON;
	LED_RED 			= LED_OFF;
	LED_GREEN 			= LED_ON;
};

void ent_ready()
{
	machine_state		= READY;
	stateS 				= &state_ready;
	//led coding state
	LED_BLUE 			= LED_OFF;
	LED_RED 			= LED_OFF;
	LED_GREEN 			= LED_ON;
};
