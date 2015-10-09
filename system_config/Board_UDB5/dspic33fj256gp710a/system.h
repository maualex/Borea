/******************************************************************************
*
*                           System Header File
*
*******************************************************************************
* FileName:           system_config.h
* Processor:          dsPIC33FJ256GP710A
* Compiler:           XC16
* Company:            Space & Precision Group, Politecnico di Torino
*
*
*******************************************************************************/

#ifndef _BOREA_H_
#define _BOREA_H_

#include <p33FJ256GP710A.h>
#include <stdint.h>
#include <stdbool.h>
#include "receiver.h"
#include "sensors.h"
#include "config.h"
#include "ref_gen.h"
#include "drv_spi.h"

// Definition for system clock
#define SYS_CLK_FrequencySystemGet()               40000000
// Definition for peripheral clock
#define SYS_CLK_FrequencyPeripheralGet()           SYS_CLK_FrequencySystemGet()
// Definition for instruction clock
#define SYS_CLK_FrequencyInstructionGet()          (SYS_CLK_FrequencySystemGet() / 2)

//data to write into the SD card at each control step
#define USE_SD_CARD_LOG
#define NFLOAT_TO_WRITE_SD  105
#define NBYTE_TO_WRITE_SD   (4*NFLOAT_TO_WRITE_SD+1)    //421 byte
#define PACKET_OFFSET_1     15                      //15 variabili
#define PACKET_OFFSET_2     (PACKET_OFFSET_1 + 14)  //14 variabili
#define PACKET_OFFSET_3     (PACKET_OFFSET_2 + 24)  //24 variabili
#define PACKET_OFFSET_4     (PACKET_OFFSET_3 + 30)  //30 variabili
                                                    //22 variabili
//                                           totale: 105 variabili

//UART X-Bee
#define BAUD_RATE_SETTING   38400    //baud rate for Xbee comunication 


//LED
#define LED_RED             LATEbits.LATE1
#define LED_GREEN           LATEbits.LATE2  //communication led
#define LED_YELLOW          LATEbits.LATE3  //for error communications
#define LED_BLUE            LATEbits.LATE4
#define ALL_LED_OFF         0b0000000000011110
#define LED_ON              0
#define LED_OFF             1
#define UP                  1
#define DOWN                0
#define ON                  1
#define OFF                 0
#define true                1
#define false               0

//CONTROL COMMANDS
#define FZ 0
#define TX 1
#define TY 2
#define TZ 3

// BODY AXIS
#define X 0
#define Y 1
#define Z 2

//questi devono essere enum!

//VERTICAL GUIDANCE STATES
#define ONTARGET     		0
#define ACCELERATION 		1
#define MAXSPEED     		2
#define BREAK        		3
#define NEARTARGET   		4

//COMMAND(FROM GROUND STATION) DECODING
#define GS_CALIBRATION_ON  	1
#define GS_CONTROL_ON      	2
#define GS_TARGET          	3
#define GS_RESET           	4
#define GS_SHUT_DOWN       	5
#define GS_THRUST_UP       	6
#define GS_THRUST_DOWN     	7
#define GS_CLOSE_FILE       8
#define GS_GET_STATUS       9

// VARIABLES FOR THE ATTITUDE STATE PREDICTOR 1
#define N_STATE_OBS1 	6
#define XC1 			0			// ANGLE
#define XC2 			1			// D_ANGLE
#define XC3 			2			// DD_ANGLE
#define XD1 			3
#define XD2 			4
#define XQ1 			5 

#define Reset() {__asm__ volatile ("reset");}   //reset del microcontroller

#define _ISR_NOPSV __attribute__((interrupt, no_auto_psv));

//extern SYSTEM_STATE machine_state;

// System initialization function
void SYSTEM_Initialize (void);

// User-defined function to set the chip select for our example drive
void USER_SdSpiSetCs_1 (uint8_t a);
// User-defined function to get the card detection status for our example drive
bool USER_SdSpiGetCd_1 (void);
// User-defined function to get the write-protect status for our example drive
bool USER_SdSpiGetWp_1 (void);
// User-defined function to initialize tristate bits for CS, CD, and WP
void USER_SdSpiConfigurePins_1 (void);


/*____________________________FUNCTION PROTOTYPES__________________________________*/

void subtracVec(float* a1, float* a2, float* result);                                    //result = a1 - a2                              
void prodsum(const float* scalar, float* v, float* a1, float* result);                  //result = scalar * v + a1
void prodsumsum(const float* scalar, float* v, float* a1, float* a2, float* result);   //result = scalar * v + a1 + a2
void Quest(float* stima_Gb, float* stima_Acc, float* quaternion);

void config(void);
int ReadMagneto(int*);
int ConfigMagneto(void);
int tx_to_slave(unsigned char*,char*,int); //invio dati allo slave

void vertical_guidance(float* pz, float* vz, float* az);
void module_config(void);
void send(unsigned char *in, unsigned char n_bytes);
void prepare_msg(float *data_float, unsigned int control_step, unsigned char state, unsigned char *data_to_be_send, unsigned char n_var);

//GPS receiver functions
void tx_to_gps(void);
void rx_from_gps(void);
char busy_uart_gps(void);
void write_uart_gps(unsigned int data);

#endif
