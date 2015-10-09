//REMARK: max 18 bytes can be send to the ground station at 50Hz

// DIGITS
#define DIGITS1	10
#define DIGITS2	100
#define DIGITS3	1000
#define DIGITS4	10000
#define DIGITS5	100000
#define DIGITS6	1000000
#define DIGITS7	10000000
#define DIGITS8	100000000

//note: uncomment what you want to send

//#define SEND_IMU_ACC 				// 3 variables
#define SEND_IMU_GYR				// 3 variables
//#define SEND_EXT_GYR				// 3 variables
//#define SEND_THRUST_PROPELLERS	// 4 variables
#define SEND_UC					// 4 variables
//#define SEND_OBS_2					// 8 variables
//#define SEND_CONTROL_2				// 7 variables
//#define SEND_BATTERY				// 1 variable
//#define SEND_CPU_LOAD				// 1 variable
#define SEND_ATTITUDE				// 3 variables
//#define SEND_RC						// 10 variables
#define SEND_PROVAVOLO

#define N_DATA                  54          //number of bytes to send in one message equals to xbee packet dimention
#define MSG_BYTE_BEGIN          100         //3 byte per rilevare inizio messaggio
#define MSG_BYTE_END            200         //3 byte per rilevare fine messaggio
#define TIMEOUT                 100         //time: 1.6us*100=0.16ms
#define TX_BUFF_LENGTH          4           //UART buffer lenght in bytes

// variables that are not defined in this library
extern long int control_step;    	// control step counter

// global variables
extern char pc_msg[4];
extern unsigned char message_to_send[5];
extern unsigned char  flag_message_send;

// prototypes
//this function always add 3 bytes at the end: 2 for control step variable and 1 for machine state variable 
void prepare_msg(float *data_float, unsigned int control_step, unsigned char state, unsigned char *data_to_be_send, unsigned char n_var);

//send N_FLOAT_DATA*4 + 3 bytes
unsigned char send2gs(unsigned char *data_to_be_send);

void prepare_msg2(int *data_int, unsigned int control_step, unsigned char state, unsigned char *data_to_be_send, unsigned char n_var);
void comm_init(void);
unsigned char send_msg(int* data);
