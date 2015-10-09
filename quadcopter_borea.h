#ifndef _BOREA_H_
#define _BOREA_H_


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

//ricevitore GPS
void tx_to_gps(void);
void rx_from_gps(void);
char busy_uart_gps(void);
void write_uart_gps(unsigned int data);


#endif
