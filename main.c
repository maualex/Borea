/******************************************************************************
*
*                           Main project File
*
*******************************************************************************
* FileName:           main.c
* Processor:          dsPIC33FJ256GP710A
* Compiler:           XC16
* Company:            Space & Precision Group, Politecnico di Torino
*
*
* Questa unità di controllo stabilizza soltanto l'assetto, il comando verticale è in open-loop.
* Il controllo di assetto si basa su un modello lineare a 3 stati dove uno stato rappresenta
* la costante di tempo dell'attuatore (83ms). Ciò comporta che la misura del giroscopio, e anche
* il comando, devono essere opportunamente trasformati in funzione della cinematica di assetto
* quindi in funzione alla sequenza di assetto scelta (qui 123).
* 
* piccolo appunto: 
* prototipo -> f(float* vettore, float* scalare)
* chiamata alla funzione     -> f(vettore, &scalare)
* ovvero con gli array viene passato direttamente l'indirizzo mentre con lo scalare
* bisogna esplicitamente passare l'indirizzo della variabile
* Nel corpo della funzione il vettore si usa normalmente vettore[.] = .. 
* mentre per lo scalare bisogna indicare il valore puntato essendo il nome 
* un indirizzo -> *(scalare) = ..
 * 
 * ATTENZIONE: il buffer per i dati da scrivere su file viene allocato dinamicamente
 * quindi bisogna impostare il linker con la giusta quantità di memoria heap
*******************************************************************************/


#include "fileio.h"     //funzioni della libreria del filesystem
#include "sd_spi.h"
#include <string.h>
#include <xc.h>
//#include <stdint.h>
//#include <stdbool.h>
#include <stdio.h>
#include "system.h"
#include "filter.h"
#include "filter_vibra.h" 
//#include "sensors.h"
#include "spi.h"
#include "attitude.h"
#include "quadcopter_borea.h"
//#include "config.h"
#include "SPA_Matrix.h"
#include "propellers.h"
#include "comm.h"
#include "MPU6000_lib.h"
#include "ADC_lib.h"
#include "PID_gains.h"
//#include "receiver.h"
//#include "ref_gen.h"
#include "state_machine.h"
#include "UC4_params.h"
#include <math.h>
#include <stdlib.h>

                                /* STRUCTURES FOR THE FILE SYSTEM */

// GetTimestamp will be called by the FILEIO library when it needs a timestamp for a file
//void GetTimestamp (FILEIO_TIMESTAMP * timeStamp);
// Helper function to initialize the RTCC from the compiler timestamp
//void RTCCInit (void);

// The sdCardMediaParameters structure defines user-implemented functions needed by the SD-SPI fileio driver.
// The driver will call these when necessary.  For the SD-SPI driver, the user must provide
// parameters/functions to define which SPI module to use, Set/Clear the chip select pin,
// get the status of the card detect and write protect pins, and configure the CS, CD, and WP
// pins as inputs/outputs (as appropriate).
// For this demo, these functions are implemented in system.c, since the functionality will change
// depending on which demo board/microcontroller you're using.
// This structure must be maintained as long as the user wishes to access the specified drive.
FILEIO_SD_DRIVE_CONFIG sdCardMediaParameters =
{
    1,                                  // Use SPI module 2
    USER_SdSpiSetCs_1,                  // User-specified function to set/clear the Chip Select pin.
    USER_SdSpiGetCd_1,                  // User-specified function to get the status of the Card Detect pin.
    USER_SdSpiGetWp_1,                  // User-specified function to get the status of the Write Protect pin.
    USER_SdSpiConfigurePins_1           // User-specified function to configure the pins' TRIS bits.
};

// The gSDDrive structure allows the user to specify which set of driver functions should be used by the
// FILEIO library to interface to the drive.
// This structure must be maintained as long as the user wishes to access the specified drive.
const FILEIO_DRIVE_CONFIG gSdDrive =
{
    (FILEIO_DRIVER_IOInitialize)FILEIO_SD_IOInitialize,                     // Function to initialize the I/O pins used by the driver.
    (FILEIO_DRIVER_MediaDetect)FILEIO_SD_MediaDetect,                       // Function to detect that the media is inserted.
    (FILEIO_DRIVER_MediaInitialize)FILEIO_SD_MediaInitialize,               // Function to initialize the media.
    (FILEIO_DRIVER_MediaDeinitialize)FILEIO_SD_MediaDeinitialize,           // Function to de-initialize the media.
    (FILEIO_DRIVER_SectorRead)FILEIO_SD_SectorRead,                         // Function to read a sector from the media.
    (FILEIO_DRIVER_SectorWrite)FILEIO_SD_SectorWrite,                       // Function to write a sector to the media.
    (FILEIO_DRIVER_WriteProtectStateGet)FILEIO_SD_WriteProtectStateGet,     // Function to determine if the media is write-protected.
};



#define N_GAINS_OBS1 6
float obs1_xp[N_STATE_OBS1], obs1_x[N_STATE_OBS1];        	// state vector
float obs1_me, obs1_y;
/////////////////////////////////////////////

// VARIABLES FOR THE ATTITUDE STATE PREDICTOR 2
#define N_STATE_OBS2 6
float obs2_xp[N_STATE_OBS2], obs2_x[N_STATE_OBS2];        	// state vector
float obs2_me, obs2_y;
/////////////////////////////////////////////

// VARIABLES FOR THE ATTITUDE STATE PREDICTOR 3
#define N_STATE_OBS3 6
float obs3_xp[N_STATE_OBS3], obs3_x[N_STATE_OBS3];        	// state vector
float obs3_me, obs3_y;
/////////////////////////////////////////////

// ref gen
float ref1_x[REF1_NX], ref1_xp[REF1_NX], ref1_u;
float ref2_x[REF1_NX], ref2_xp[REF1_NX], ref2_u;
float ref3_x[REF1_NX], ref3_xp[REF1_NX], ref3_u;

// IMU MPU6000 buffers
int imu_buf_ax[IMU_BUFFER_LENGTH]; 
int imu_buf_ay[IMU_BUFFER_LENGTH]; 
int imu_buf_az[IMU_BUFFER_LENGTH]; 
int imu_buf_gx[IMU_BUFFER_LENGTH];  			
int imu_buf_gy[IMU_BUFFER_LENGTH];
int imu_buf_gz[IMU_BUFFER_LENGTH];
int imu_buf_te[IMU_BUFFER_LENGTH]; 

// IMU measurements
float imu_mis_a[3], imu_mis_g[3], imu_mis_t;

// external measurement buffers
int ext_buf_gx[EXT_BUFFER_LENGTH];                  //external gyroscope X-axis
int ext_buf_gy[EXT_BUFFER_LENGTH];                  //external gyroscope Y-axis
int ext_buf_gz[EXT_BUFFER_LENGTH];                  //external gyroscope Z-axis
int ext_buf_vb[EXT_BUFFER_LENGTH];				    //battery voltage
int ext_buf_sn[EXT_BUFFER_LENGTH];                  //ultrasound voltage

// buffer pointers for external measurements
int *pgx, *pgy, *pgz, *pb, *ps;

// generic global variables
float ext_mis_g[3];                 // external gyroscope measuments at 400Hz
float imu_omega[3]; 
float ext_omega[3]; 
float omega[3];                   // gyro measurements for attitude flag_control_unit_period
float attitude[3];                  // gyro measurements for attitude flag_control_unit_period
float ext_mis_sn;                   // ultrasound measument at 50Hz
float ext_mis_vb;                   // battery voltage measument at 50Hz
float ext_gyro_bias[3];             // bias calculated in the flag_calibration phase 
float imu_gyro_bias[3];             // bias calculated in the flag_calibration phase

//quaternion variables
float q[4];                         // 
float q_p[4];                       // 
float q0[4];                        // 
float delta_quat[4];                // 
float qnorm;                        // 

float gravity[3];                   // gravity vector (body)
float vertical_thrust;
float g_compensation;
float vertical_command;             // [m]
float gs_thrust;
float target[3];
float prop_omg[4];					//propeller angular rate [rad/s]
float thrust[4];					//propeller thrust [N]

//control
unsigned int ud[4], ud_p[4], ud0[4] = {UD_OFFSET, UD_OFFSET, UD_OFFSET, UD_OFFSET};                 //digital duty cycle
float uc[4];						//flag_control_unit_period requirements [FZ,TX,TY,TZ]
float uc_emc[3];
float u[4];    						//continuous time duty cycle command
float control1_err[3], control2_err[3], control3_err[3];
float control1_uc_p, 	control2_uc_p, 	control3_uc_p;
float control1_uc, 	control2_uc, 	control3_uc;
float control1_b, 	control2_b, 	control3_b;
float control1_fb, 	control2_fb, 	control3_fb;
float control1_ref, 	control2_ref, 	control3_ref;
float control1_dist, 	control2_dist, 	control3_dist;
//float control2_K[3];

long int control_step;              //32 bit control step counter
unsigned int media_gyro;           	//contatore per media gyro
unsigned char gyro_error    = 0;
unsigned char error_gyroX   = 1;    //error code in gyro reading
unsigned char error_gyroY   = 1;    //error code in gyro reading
unsigned char error_gyroZ   = 1;    //error code in gyro reading


unsigned char flag_control_unit_period;
unsigned int timeout_switch_on;
unsigned char rc_error[RC_CHANNELS];		// error from receiver

//for communication
int message[N_DATA];


//Receiver variables
float cambio_ang;
float RC_thrust, RC_thrust_p, RC_angle1, RC_angle1_p, RC_angle2, RC_angle2_p, RC_angle3, RC_gain, RC_offset, RC_beta;
float RC_in1, RC_in2, RC_in3;
float RC_temp = 0;
unsigned char RC_c1 = 0;
unsigned int pp 	= 0;
//const int N    	    = 500;
//#define   Nn 		500
float *p1, *p2, *p3, *p4, *p5, *p6;

            
//buffers pointer for butterworth filter
double *gx_IN;
double *gy_IN;
double *gz_IN;
double *gx_OUT;
double *gy_OUT;
double *gz_OUT;
double bufferIN[30];		//butterworth buffer
double bufferOUT[30]; 		//butterworth buffer

// PID variables
float Xpid_err, Xpid_int, Xpid_int_p;
float Ypid_err, Ypid_int, Ypid_int_p;
float Zpid_err, Zpid_int, Zpid_int_p;

//attitude
float th, psi;
float cth_inv;

//per la ricostruzione del comando
float rec_u[4], rec_prop_omg[4], rec_thrust[4], rec_uc[4], uu, oo;

//per il nome del file
int n_test;
char file_test_name[10];

//prototypes
void control_init(void); 
void init(void);
void update_state(void);
void gs_decoding(void);
//utility function
void serialize_float(float *data_int, unsigned char *data, uint8_t n_var);

//per calcolare bias nella calibrazione
float media1[3], media2[3], media3[3];     //3 variabli 3 


float mag[3] = {1,2,3};
float gps[3] = {1,2,3};

unsigned char* data_to_write;                   // array di byte da scrivere su SD
float data_in[NFLOAT_TO_WRITE_SD];              // misure
unsigned char flag_file_created;
unsigned char message_to_send[5];
unsigned char  flag_message_send;
extern unsigned char data_to_be_send[N_DATA];

void crea_file(void);
void data_packaging(void);
FILEIO_OBJECT file;

int main(void)
{   
   	//local variables
	unsigned int i;	

        SYSTEM_Initialize();        //setta il clock e accende led rosso
    
    //serialize_float(misure, datalog, 3);

    n_test = 1; 

    #ifdef USE_SD_CARD_LOG
        // Initialize the library
        if (!FILEIO_Initialize())
        {
            while(1);
        }
        // Loop on this function until the SD Card is detected.
        if (FILEIO_MediaDetect(&gSdDrive, &sdCardMediaParameters) == 1)
        {
            //media detected
            //demoState = DEMO_STATE_MEDIA_DETECTED;
        }


        FILEIO_ERROR_TYPE error;
        error = FILEIO_DriveMount('A', &gSdDrive, &sdCardMediaParameters);
        if ( error == FILEIO_ERROR_NONE)
        {
            //demoState = DEMO_STATE_DRIVE_MOUNTED;

        }
        else
        {
            //demoState = DEMO_STATE_NO_MEDIA;
            return 0;
        }
    #endif
    
    
    data_to_write = (unsigned char*)malloc(NBYTE_TO_WRITE_SD);	// 
    
	init();
	
/*	
-----------------------------------------------------------------------------------------------
---------------------------------- I N F I N I T Y   L O O P ----------------------------------
-----------------------------------------------------------------------------------------------
*/ 	
    while(1)
    {
	
	//________________________________C O N T R O L   R O U T I N E__________________________________//
	//_______________________________________________________________________________________________//

        if(flag_control_unit_period == ON)		//main loop triggered at 50Hz by timer 1
        {
			/*
			nota: la lettura della ricevente e la misura dei sensori vengono sempre effettuate
			indipendentemente dallo stato della macchina
			*/
			control_step++;      				// increment control step counter 
			
			
//:::::::::: R E C E I V E R ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::// 
			
			//REMARK: ad ogni ciclo il primo task consiste nella lettura della ricevente e della ground station (gs)
			
			//receiver channel updating
			RC_thrust   = RC_thrust_p;
			RC_angle1   = RC_angle1_p;
			RC_angle2   = RC_angle2_p;
			
			rc_in_duty_cycle(rc_error);			// reading channels
			
			//saturation: OGNI CANALE DEVE AVERE LA PROPRIA SATURAZIONE!!
			for(i=0;i<RC_CHANNELS;i++)
			{
				if(rc_channel_value[i]>RC_SAT_MAX) rc_channel_value[i] = RC_SAT_MAX;
				if(rc_channel_value[i]<RC_SAT_MIN) rc_channel_value[i] = RC_SAT_MIN;
			}
			
			
			//rc_ch_conditioning(): rende il comando dello stick simmetrico e lo normalizza
			//calcolo corsa destra e sinistra dello stick
			//parametro di input	: 'ch0' 	-> valore digitale centro stick
			//parametro di input	: 'x'   	-> ingresso digitale 
			//parametro di output	: 'rc_ch' 	-> uscita normalizzata e simmetrica [-1,+1]
			//costanti di input 	: valore digitale massimo e minimo dello stick
			
            //CHANNEL 1 -> VERTICAL THRUST 
            if(rc_channel_value[0]<RC_CH1_MIN) rc_channel_value[0] = RC_CH1_MIN;
            if(rc_channel_value[0]>RC_CH1_MAX) rc_channel_value[0] = RC_CH1_MAX;
			RC_in1 = (rc_channel_value[0] - RC_CH1_MIN)*RC_CH1_NORMALIZE;
            
			RC_in2 = RC_2_GAIN*rc_ch_conditioning(rc_channel_value[RC_2], RC_ZERO_CH2);	//+-10°
			RC_in3 = RC_3_GAIN*rc_ch_conditioning(rc_channel_value[RC_3], RC_ZERO_CH3); //+-10°  questo è il joystick verticale destro
			

//centering (A COSA SERVE SUL CANALE1??)
			//if(RC_in1<RC_STICK_CENTERING && RC_in1>-RC_STICK_CENTERING ) RC_in1 = 0;
			
            //if(RC_in2<RC_STICK_CENTERING && RC_in2>-RC_STICK_CENTERING ) RC_in2 = 0;
			//if(RC_in3<RC_STICK_CENTERING && RC_in3>-RC_STICK_CENTERING ) RC_in3 = 0;

			//filtering
			RC_thrust_p = (1-RC_1_BETA)*RC_thrust + RC_1_BETA*RC_1_GAIN*RC_in1; 
			RC_angle1_p = (1-RC_2_BETA)*RC_angle1 + RC_2_BETA*RC_in2;	
			RC_angle2_p = (1-RC_3_BETA)*RC_angle2 + RC_3_BETA*RC_in3;
/********************************************************************************************************************/			
            
            
            
//LOCK & UNLOCK RECEIVER LOGIC
            if(RC_thrust<RC_1_MIN_THRUST)
            {
               if(RC_angle2>(0.7*RC_3_GAIN) && RC_angle2<=RC_3_GAIN && flag_actuator==OFF)
                {
                    timeout_switch_on++;
                    if(timeout_switch_on>100) 
                    {
                        flag_actuator 		= ON;		//accendo il controllo
                        timeout_switch_on 	= 0;
                    }
                }
                // actuator lock
                if(RC_angle2<-(0.7*RC_3_GAIN) && RC_angle2>-RC_3_GAIN && flag_actuator==ON)
                {
                    timeout_switch_on++;
                    if(timeout_switch_on>50) 
                    {
                        flag_actuator 		= OFF;		//spengo il controllo
                        timeout_switch_on 	= 0;
                    }
                }

                // calibration request
                if(RC_angle2<-(0.7*RC_3_GAIN) && RC_angle2>-RC_3_GAIN && flag_actuator==OFF)
                {
                    timeout_switch_on++;
                    if(timeout_switch_on>50) 
                    {
                        flag_calibration_request	= ON;		
                        timeout_switch_on 			= 0;
                    }
                } 
            }
			

			gs_decoding();		//check for a new ground station command
			
			
//:::::::::: M E A S U R E M E N T S :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::://   
			
			//REMARK: tutta la sensoristica (IMU compresa) viene sempre letta
			
			//measurements reset
			for(i=0;i<3;i++)
			{
				imu_mis_a[i] = 0;
				imu_mis_g[i] = 0;
				ext_mis_g[i] = 0;
			}
			imu_mis_t 	= 0;
			ext_mis_sn  = 0;
			ext_mis_vb  = 0;
			

			//	SET_AND_SAVE_CPU_IPL(current_cpu_ipl,7);	// zona critica
			
            float gyro_mis_f, gyro_mis;
            
			for(i=0;i<IMU_BUFFER_LENGTH;i++)  //Average of 20 samples from shared buffer
			{
				imu_mis_a[IMU_ACC_AXIS_X] += (float) IMU_ACC_SIGN_X imu_buf_ax[i]*IMU_ACC_SCALE*0.05; 	// Conversion to m/s^2 and average
				imu_mis_a[IMU_ACC_AXIS_Y] += (float) IMU_ACC_SIGN_Y imu_buf_ay[i]*IMU_ACC_SCALE*0.05; 	// Conversion to m/s^2 and average
				imu_mis_a[IMU_ACC_AXIS_Z] += (float) IMU_ACC_SIGN_Z imu_buf_az[i]*IMU_ACC_SCALE*0.05; 	// Conversion to m/s^2 and average	 
				imu_mis_g[IMU_GYR_AXIS_X] += (float) IMU_GYR_SIGN_X imu_buf_gx[i]*IMU_GYR_SCALE*0.05;	// Conversion to rad/s and average
				
                imu_mis_g[IMU_GYR_AXIS_Y] += (float) IMU_GYR_SIGN_Y imu_buf_gy[i]*IMU_GYR_SCALE*0.05;   // Conversion to rad/s and average
//				gyro_mis        = (float) IMU_GYR_SIGN_Y imu_buf_gy[i]*IMU_GYR_SCALE;
//              gyro_mis_f      = filter_form1_FAST(gyro_mis, num1, den1, order1, gx_IN, gx_OUT);
//              imu_mis_g[IMU_GYR_AXIS_Y] += gyro_mis_f*0.05;
                
                imu_mis_g[IMU_GYR_AXIS_Z] += (float) IMU_GYR_SIGN_Z imu_buf_gz[i]*IMU_GYR_SCALE*0.05;   // Conversion to rad/s and average	
				imu_mis_t    			  += (float) imu_buf_te[i]*0.05;	
			}
				
			for(i=0;i<EXT_BUFFER_LENGTH;i++)    
			{
				ext_mis_g[EXT_GYR_AXIS_X] += (float) EXT_GYR_SIGN_X ext_buf_gx[i]*EXT_GYR_SCALE*0.125;
				ext_mis_g[EXT_GYR_AXIS_Y] += (float) EXT_GYR_SIGN_Y ext_buf_gy[i]*EXT_GYR_SCALE*0.125;
				ext_mis_g[EXT_GYR_AXIS_Z] += (float) EXT_GYR_SIGN_Z ext_buf_gz[i]*EXT_GYR_SCALE*0.125;  
				ext_mis_sn	 			  += (float) ext_buf_sn[i]*ADC_12BIT_CONVERTION*0.125; 
				ext_mis_vb   			  += (float) ext_buf_vb[i]*EXT_BAT_SCALE*EXT_BAT_SCALE_CORRECTION*0.125; 
			} 
			
			//	RESTORE_CPU_IPL(current_cpu_ipl); 
			ext_mis_sn	= EXT_SON_SCALE*(ext_mis_sn + EXT_SON_OFFSET);	//[m]
			ext_mis_vb	= ext_mis_vb + EXT_BAT_OFFSET;	//[V]
			ext_mis_vb	= filter_form1_FAST(ext_mis_vb, num, den, order, gx_IN, gx_OUT);

            			
		

//:::::::::: S E N S O R   C A L I B R A T I O N :::::::::::::::::::::::::::::::::::::::::::::::://		

			if(flag_calibration == ON)
			{
				if(media_gyro<CALIBRATION_COUNTER)
				{
					for(i=0;i<3;i++)
					{
                        media1[i] += imu_mis_g[i];
                        media2[i] += imu_mis_a[i];
                        media3[i] += ext_mis_g[i];
                        
						imu_gyro_bias[i] += imu_mis_g[i];
						ext_gyro_bias[i] += ext_mis_g[i];
						gravity[i] 		 += imu_mis_a[i];
					}
					media_gyro++;
				}
				else
				{
					for(i=0;i<3;i++)
					{
						ext_gyro_bias[i] = media3[i]/CALIBRATION_COUNTER;
						imu_gyro_bias[i] = media1[i]/CALIBRATION_COUNTER;
                        
                        #if defined GRAVITY_VECTOR
                            gravity[i]   = media2[i]/CALIBRATION_COUNTER; 
                        #else
                            gravity[i]   = 0;
                        #endif
					}
                    #ifndef GRAVITY_VECTOR
                        gravity[2]   = GCON;
                    #endif
					

					//initial attitude
//					attitude[1] 	= -asin(gravity[0]/GCON);
//					attitude[0] 	= asin(gravity[1]/(GCON*cos(attitude[1])));
//					attitude[2] 	= 0;

//					Euler2Quat(attitude,q0,0);
					media_gyro 	  = 0;
                    for(i=0;i<3;i++)
					{
                        media1[i] = 0.0;
                        media2[i] = 0.0;
                        media3[i] = 0.0;
                    }
					flag_calibration = OFF;
				}
			}
			
			
			
			
//:::::::::: G Y R O S C O P E   B I A S   C O M P E N S A T I O N :::::::::::::::::::::::::::::://	
			
			if(flag_calibration)
			{
				for(i=0;i<3;i++)
				{
					imu_omega[i] = imu_mis_g[i];  	//IMU gyroscope [rad]
					ext_omega[i] = ext_mis_g[i];  	//external gyroscope [rad]	
					
				}
			}
			else
			{
				for(i=0;i<3;i++)
				{
					imu_omega[i] = imu_mis_g[i] - imu_gyro_bias[i];  	//IMU gyroscope [rad]
					ext_omega[i] = ext_mis_g[i] - ext_gyro_bias[i];  	//external gyroscope [rad]	
				}
			}
			
            //scelta del giroscopio
            for(i=0;i<3;i++)
            {
                #if defined IMU
                    omega[i] = imu_omega[i];
                #else
                    omega[i] = ext_omega[i];
                #endif
            }


/* **********************************************************************************************************
 * IN QUESTO PUNTO TUTTI GLI INPUT (COMANDI E MISURE) DEL CONTROLLO SONO PRONTI 
 */
	
            
            
//:::::::::: I N I T I A L I Z A T I O N :::::::::::::::::::::::::::::::::::::::::::::::::::::::://
			
            if(flag_control_init==ON) control_init();	//prepare initial conditions for the control loop
            
            
		
			
//:::::::::: C O N T R O L   L O O P :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::://	
		
			if(flag_control==ON)		// control loop
			{
			
			
//:::::::::: U P D A T E   S T A T E S   A N D   M E A S U R E M E N T S //           

				for(i=0;i<N_STATE_OBS1;i++) 						// state observer
				{
					obs1_x[i] = obs1_xp[i];
					obs2_x[i] = obs2_xp[i];
					obs3_x[i] = obs3_xp[i];
				}	    
                ref_update(ref1_xp, ref1_x);						// reference generator updating
				ref_update(ref2_xp, ref2_x);						// reference generator updating
                
                // attitude update
				for(i=0;i<4;i++) q[i] = q_p[i];						// attitude quaternion
				Quat2Euler(q, attitude,0); 							// attitude angles (123)
				th 		= attitude[1];
				psi 	= attitude[2];
                
                //command update
				for(i=0;i<4;i++) ud[i] = ud_p[i];
				          

                
//:::::::::: MEASUREMENT TRANSFORMATION
				cth_inv 		= 1/cos(th);
				obs1_y 			= TCON*cth_inv*(cos(psi)*omega[X] - sin(psi)*omega[Y]);										// [rad]
				obs2_y 			= TCON*(sin(psi)*omega[X] + cos(psi)*omega[Y]);												// [rad]
				obs3_y 			= TCON*cth_inv*(-cos(psi)*sin(th)*omega[X] + sin(psi)*sin(th)*omega[Y] + cos(th)*omega[Z]);	// [rad]

                
                
//:::::::::: COMMAND RECOVERY		
			for(i=0;i<4;i++)
			{
				rec_u[i] 			= (ud[i] - UD_OFFSET)*UD_DELTA_I;
				uu 					= rec_u[i];
				rec_prop_omg[i] 	= ESC_COEF_1_I*uu*uu*uu + ESC_COEF_2_I*uu*uu + ESC_COEF_3_I*uu + ESC_COEF_4_I*ext_mis_vb + ESC_COEF_5_I;
				oo 					= rec_prop_omg[i];
				rec_thrust[i] 		= PROP_COEF_1_I*oo*oo + PROP_COEF_2_I*oo + PROP_COEF_3_I;
			}
			
			rec_uc[TX]		= ARM_LONG*(rec_thrust[1] - rec_thrust[3]);
			rec_uc[TY]		= ARM_LONG*(rec_thrust[2] - rec_thrust[0]);
			//NOTA: la rotazione dell'elica 1 è positiva (antioraria) quindi la coppia di reazione è negativa
			rec_uc[TZ]		= KT*(rec_thrust[0] - rec_thrust[1] + rec_thrust[2] - rec_thrust[3]);	
            rec_uc[FZ]		= rec_thrust[0] + rec_thrust[1] + rec_thrust[2] + rec_thrust[3];
            
            
//:::::::::: COMMAND TRANSFORMATION             
            control1_uc		= TCON*TCON*JXINV*cth_inv*(cos(psi)*rec_uc[TX] - sin(psi)*rec_uc[TY]);
            control2_uc		= TCON*TCON*JYINV*(sin(psi)*rec_uc[TX] + cos(psi)*rec_uc[TY]);
            control3_uc		= TCON*TCON*JZINV*cth_inv*(-cos(psi)*sin(th)*rec_uc[TX] + sin(psi)*sin(th)*rec_uc[TY] + cos(th)*rec_uc[TZ]);
				
                
                
				
            
            
//:::::::::: S T A T E   P R E D I C T O R //
            
            
//EMBEDDED MODEL X-AXIS
				obs1_me 		= obs1_y - obs1_x[XC2]; 		// model error		[rad]
				obs1_xp[XC1] 	= obs1_x[XC1] + obs1_x[XC2];
				obs1_xp[XC2] 	= obs1_x[XC2] + obs1_x[XC3];  
				obs1_xp[XC3] 	= obs1_x[XC3]*(1-UC4_OBS1_BD) + (control1_uc + obs1_x[XD1] + obs1_me*UC4_OBS1_L0 + obs1_x[XQ1]*UC4_OBS1_M0)*UC4_OBS1_BD;
				obs1_xp[XD1] 	= obs1_x[XD1] + obs1_x[XD2] + obs1_x[XQ1]*UC4_OBS1_M1;
				obs1_xp[XD2] 	= obs1_x[XD2] + obs1_x[XQ1]*UC4_OBS1_M2;
				obs1_xp[XQ1] 	= obs1_x[XQ1]*(1-UC4_OBS1_BQ) + obs1_me;

				
//EMBEDDED MODEL Y-AXIS
				//modifiche per il banco prova (tolgo le trasformazioni)
                obs2_y 			= TCON*omega[Y];
                control2_uc		= TCON*TCON*JYINV*rec_uc[TY];
                control2_uc     = 0; //open-loop test
                        
				obs2_me 		= obs2_y - obs2_x[XC2]; 		// model error		[rad]
				obs2_xp[XC1] 	= obs2_x[XC1] + obs2_x[XC2];
				obs2_xp[XC2] 	= obs2_x[XC2] + obs2_x[XC3];    // omega estimation [rad]
				obs2_xp[XC3] 	= obs2_x[XC3]*(1-UC4_OBS2_BD) + (control2_uc + obs2_x[XD1] + obs2_me*UC4_OBS2_L0 + obs2_x[XQ1]*UC4_OBS2_M0)*UC4_OBS2_BD;
				obs2_xp[XD1] 	= obs2_x[XD1] + obs2_x[XD2] + obs2_x[XQ1]*UC4_OBS2_M1;
				obs2_xp[XD2] 	= obs2_x[XD2] + obs2_x[XQ1]*UC4_OBS2_M2;
				obs2_xp[XQ1] 	= obs2_x[XQ1]*(1-UC4_OBS2_BQ) + obs2_me;			

                
//EMBEDDED MODEL Z-AXIS

				obs3_me 		= obs3_y - obs3_x[XC2]; 		// model error		[rad]
				obs3_xp[XC1] 	= obs3_x[XC1] + obs3_x[XC2];
				obs3_xp[XC2] 	= obs3_x[XC2] + obs3_x[XC3];  
				obs3_xp[XC3] 	= obs3_x[XC3]*(1-UC4_OBS3_BD) + (control3_uc + obs3_x[XD1] + obs3_me*UC4_OBS3_L0 + obs3_x[XQ1]*UC4_OBS3_M0)*UC4_OBS3_BD;
				obs3_xp[XD1] 	= obs3_x[XD1] + obs3_x[XD2] + obs3_x[XQ1]*UC4_OBS3_M1;
				obs3_xp[XD2] 	= obs3_x[XD2] + obs3_x[XQ1]*UC4_OBS3_M2;
				obs3_xp[XQ1] 	= obs3_x[XQ1]*(1-UC4_OBS3_BQ) + obs3_me;
			
			
                
                
                
       
//:::::::::: A T T I T U D E   I N T E G R A T I O N //

				UpdateQuat(q, omega, q_p, TCON);    // ome is in rad/s


                
//:::::::::: R E F E R E N C E   G E N E R A T O R //
				//ho scambiato gli angoli
				
				cambio_ang = - RC_angle2;
				ref_gen(ref1_x, &cambio_ang, ref1_xp, &ref1_u);	// generatore asse X - angle 2 - stick verticale destro
				ref_gen(ref2_x, &RC_angle1, ref2_xp, &ref2_u);	// generatore asse Y - angle 1 - stick orizontale destro
				//controllo spin a zero
				ref3_x[REF1_XC1] = 0;
				ref3_x[REF1_XC2] = 0;
				ref3_x[REF1_XC3] = 0;
				ref3_u 			 = 0;
				
                
                
                
//:::::::::: C O N T R O L  L A W //

				//X-axis control
				control1_err[0] 	= ref1_x[REF1_XC1] - obs1_xp[XC1];
				control1_err[1] 	= ref1_x[REF1_XC2] - obs1_xp[XC2];
				control1_err[2] 	= ref1_x[REF1_XC3] - obs1_xp[XC3];
				control1_fb			= UC4_X_K1*control1_err[0] + UC4_X_K2*control1_err[1] + UC4_X_K3*control1_err[2];
				control1_dist		= - obs1_xp[XD1];
				control1_ref		= ref1_u;
				control1_uc_p		= control1_fb + control1_dist + control1_ref; 		// [rad]
				
				
				//Y-axis control
				control2_err[0] 	= ref2_x[REF1_XC1] - obs2_xp[XC1];
				control2_err[1] 	= ref2_x[REF1_XC2] - obs2_xp[XC2];
				control2_err[2] 	= ref2_x[REF1_XC3] - obs2_xp[XC3];
				control2_fb			= UC4_Y_K1*control2_err[0] + UC4_Y_K2*control2_err[1] + UC4_Y_K3*control2_err[2];
				control2_dist		= - obs2_xp[XD1];
				control2_ref		= ref2_u;
				control2_uc_p		= control2_fb + control2_dist + control2_ref; 		// [rad]
				
				
				//Z-axis control
				control3_err[0] 	= ref3_x[REF1_XC1] - obs3_xp[XC1];
				control3_err[1] 	= ref3_x[REF1_XC2] - obs3_xp[XC2];
				control3_err[2] 	= ref3_x[REF1_XC3] - obs3_xp[XC3];
				control3_fb			= UC4_Z_K1*control3_err[0] + UC4_Z_K2*control3_err[1] + UC4_Z_K3*control3_err[2];
				control3_dist		= - obs3_xp[XD1];
				control3_ref		= ref3_u;
				control3_uc_p		= control3_fb + control3_dist + control3_ref; 		// [rad]
				
				/*
				// take-off thrust ramp
				if(vertical_thrust < RAMP_END && THRUST_RAMP)
				{
					vertical_thrust += RAMP_SLOPE;    
				}
				else 
				{
					vertical_thrust = RAMP_END;	//Thrust can not be changed from Ground Station ( PC )
				}
				*/

// command transformation	 			
	 			uc_emc[TX]  = control1_b*(cos(psi)*cos(th)*control1_uc_p + sin(psi)*control2_uc_p);		// Tx [Nm]
	 			uc_emc[TY]  = control2_b*(-cos(th)*sin(psi)*control1_uc_p + cos(psi)*control2_uc_p);	// Ty [Nm]
	 			uc_emc[TZ]  = control3_b*(sin(th)*control1_uc_p + control3_uc_p);						// Tz [Nm]
				
                
                
				/*
				//without command transformation
				uc[TX]  = control1_b*control1_uc_p;			// Tx [Nm]
				uc[TY]  = control2_b*control2_uc_p;			// Ty [Nm]
				uc[TZ]  = control3_b*control3_uc_p;			// Tz [Nm]
				*/
				
				
				
				
// controllo PID singolo asse		

				//pid X-axis
				Xpid_int 	= Xpid_int_p;
				Xpid_err 	= cambio_ang - attitude[X];
				Xpid_int_p 	= Xpid_int + TCON*Xpid_err;
				
                RC_angle1 = 0;
				//pid Y-axis
				Ypid_int 	= Ypid_int_p;
				Ypid_err 	= RC_angle1 - attitude[Y];
				Ypid_int_p 	= Ypid_int + TCON*Ypid_err;
				
				//pid Z-axis
				Zpid_int 	= Zpid_int_p;
				Zpid_err 	= - attitude[Z];
				Zpid_int_p 	= Zpid_int + TCON*Zpid_err;
				
				
				uc[TX]  	= PID_KP_X*Xpid_err - PID_KD_X*omega[X] + PID_KI_X*Xpid_int; //[Nm]
				uc[TY]  	= PID_KP_Y*Ypid_err - PID_KD_Y*omega[Y] + PID_KI_Y*Ypid_int; //[Nm]
				uc[TZ]  	= -(PID_KP_Z*Zpid_err - PID_KD_Z*omega[Z] + PID_KI_Z*Zpid_int); //[Nm]
                
                //comando EMC
//				uc[TX] = uc_emc[TX];
//              uc[TY] = uc_emc[TY];
//              uc[TZ] = uc_emc[TZ];
                
                
				#ifdef OPEN_LOOP
				{
					uc[TX] = 0;
					uc[TY] = 0;
					uc[TZ] = 0;
				}
				#endif
				
				//open-loop vertical thrust
				uc[FZ] 	= FZ_INIT; //RC_thrust + FZ_INIT; 				// VERTICAL THRUST [N]
                
                
                
                
                
//:::::::::: A C T U A T O R // 

//DISPATCHING//
			
				thrust[P1] = 0.25*uc[FZ] - uc[TY]*0.5*ARM_LONG_I - uc[TZ]*KT_I*0.25;
				thrust[P2] = 0.25*uc[FZ] + uc[TX]*0.5*ARM_LONG_I + uc[TZ]*KT_I*0.25;
				thrust[P3] = 0.25*uc[FZ] + uc[TY]*0.5*ARM_LONG_I - uc[TZ]*KT_I*0.25;
				thrust[P4] = 0.25*uc[FZ] - uc[TX]*0.5*ARM_LONG_I + uc[TZ]*KT_I*0.25;
							  
				for(i=0;i<4;i++)	//idem for each propeller
				{
					
//THRUST SATURATIONS
					if(thrust[i]>F_MAX) thrust[i] = F_MAX;
					if(thrust[i]<F_MIN) thrust[i] = F_MIN;

//ACTUATOR MODEL ('ILLINOIS') -> FROM THRUST TO PROPELLER ANGULAR RATE
					prop_omg[i] 	= PROP_COEF_1*thrust[i]*thrust[i] + PROP_COEF_2*thrust[i] + PROP_COEF_3; 	// [rad/s]
					
//ESC MODEL -> DUTY CYCLE COMPUTATION
					float omg;
					omg = prop_omg[i];
					
					u[i] = ESC_COEF_1*omg*omg*omg + ESC_COEF_2*omg*omg + ESC_COEF_3*omg + ESC_COEF_4*ext_mis_vb;	//duty cycle [0-1]

	
//DUTY CYCLE DISCRETIZATION
					ud_p[i] = (unsigned int)(u[i]*UD_DELTA) + UD_OFFSET;
				}		

//PWM MODULES SETTING
                #ifdef ACTUATORS
				{
					prop_set(ud_p);	//set PWM duty cycle
				}
				#endif
				
                
                
                
                
                
                //:::::::::: D A T A  L O G ---- S D - C A R D :::::::::::::::::::::::::::::::::::::::::::::::::://
                #ifdef USE_SD_CARD_LOG
                    data_packaging();

                    serialize_float(data_in, data_to_write, NFLOAT_TO_WRITE_SD);

                    //the last byte in the buffer is dedicated for the machine state
                    data_to_write[NBYTE_TO_WRITE_SD-1] = machine_state;

                    if (FILEIO_Write (data_to_write, 1, NBYTE_TO_WRITE_SD, &file) != NBYTE_TO_WRITE_SD)
                    {
                        //WRITE FAILED;
                        LED_YELLOW = LED_ON;
                    }
                    else
                    {
                        LED_YELLOW = LED_OFF;
                    }
                #endif
            
			}	//end control loop
			

                
                

                
                
//:::::::::: D A T A  T R A S M I S S I O N ::::::::::::::::::::::::::::::::::::::::::::::::::::://

			/*
			nota: senza Xbee si riescono a inviare 21 byte senza "mai" errori
			con Xbee si inviano 18 bytes ed ogni tanto sbaglia
			*/
			
			#ifdef DATA_TRASMISSION
			{
				/* 1. measurements packaging */ 
				
				int index = 0;

				#ifdef SEND_IMU_ACC
				{
					//accelerometer measurement
					message[index]	 = (int)(imu_mis_a[0]*DIGITS3);		// ACCELEROMETER
					index++;
					message[index]	 = (int)(imu_mis_a[1]*DIGITS3);		// ACCELEROMETER
					index++;
					message[index]	 = (int)(imu_mis_a[2]*DIGITS3); 	// ACCELEROMETER
					index++;
				}
				#endif
				
				
				#ifdef SEND_IMU_GYR
				{
					//gyroscope measurement
					message[index]   = (int)(imu_omega[0]*DIGITS4); 	// GYROSCOPE
					index++;
					message[index]   = (int)(imu_omega[1]*DIGITS4); 	// GYROSCOPE
					index++;
					message[index]   = (int)(imu_omega[2]*DIGITS4); 	// GYROSCOPE
					index++;
				}
				#endif
				
				#ifdef SEND_EXT_GYR
				{
					//gyroscope measurement
					message[index]   = (int)(ext_omega[0]*DIGITS4); 	// GYROSCOPE
					index++;
					message[index]   = (int)(ext_omega[1]*DIGITS4); 	// GYROSCOPE
					index++;
					message[index]   = (int)(ext_omega[2]*DIGITS4); 	// GYROSCOPE
					index++;
				}
				#endif
				
				#ifdef SEND_THRUST_PROPELLERS
				{
					//propeller thrust
					message[index]  = (int)(thrust[P1]*DIGITS2);		// THRUST F1
					index++;
					message[index]  = (int)(thrust[P2]*DIGITS2);		// THRUST F2
					index++;
					message[index]  = (int)(thrust[P3]*DIGITS2);		// THRUST F3
					index++;
					message[index]  = (int)(thrust[P4]*DIGITS2);		// THRUST F4
					index++;
				}
				#endif
				
				#ifdef SEND_UC
				{
					//command requirements
					message[index]  = (int)(uc[FZ]*DIGITS2);			// FZ
					index++;
					message[index]  = (int)(uc[TX]*DIGITS4);			// TX
					index++;
					message[index]  = (int)(uc[TY]*DIGITS4);			// TY
					index++;
					message[index]  = (int)(uc[TZ]*DIGITS4);			// TZ
					index++;
				}
				#endif
				
				#ifdef SEND_OBS_2		//8 variables
				{
					//observer variables
					message[index]  = (int)(obs2_y*DIGITS6);			
					index++;
					message[index]  = (int)(obs2_me*DIGITS7);		
					index++;
					message[index]  = (int)(obs2_xp[XC1]*DIGITS5); 		
					index++;
					message[index]  = (int)(obs2_xp[XC2]*DIGITS6);
					index++;
					message[index]  = (int)(obs2_xp[XC3]*DIGITS6);
					index++;
					message[index]  = (int)(obs2_xp[XD1]*DIGITS6);
					index++;
					message[index]  = (int)(obs2_xp[XD2]*DIGITS6);
					index++;
					message[index]  = (int)(obs2_xp[XQ1]*DIGITS6);
					index++;
				}
				#endif
				
				#ifdef SEND_CONTROL_2		//7 variables
				{
					message[index]  = (int)(control2_err[0]*DIGITS5);
					index++;
					message[index]  = (int)(control2_err[1]*DIGITS5);
					index++;
					message[index]  = (int)(control2_err[2]*DIGITS7);
					index++;
					message[index]  = (int)(control2_fb*DIGITS6);
					index++;
					message[index]  = (int)(control2_dist*DIGITS5);
					index++;
					message[index]  = (int)(control2_ref*DIGITS5);
					index++;
					message[index]  = (int)(control2_uc_p*DIGITS6);
					index++;
				}
				#endif
				
				#ifdef SEND_BATTERY
				{
					//battery voltage
					message[index]  = (int)(ext_mis_vb*DIGITS2);	// Vb
					index++;
				}
				#endif
				
				#ifdef SEND_CPU_LOAD
				{
					//cpu load
					message[index]  = (int)(TMR1);
					index++;
				}
				#endif
				
				#ifdef SEND_ATTITUDE
				{
					//attitude
					message[index]  = (int)(attitude[0]*DIGITS4);  	// ATTITUDE
					index++;
					message[index]  = (int)(attitude[1]*DIGITS4);  	// ATTITUDE
					index++;
					message[index]  = (int)(attitude[2]*DIGITS4);  	// ATTITUDE
					index++;
				}
				#endif
				
				#ifdef SEND_RC		// 10 variables
				{
					message[index]	 = rc_channel_value[0];	
					index++;
					message[index]	 = rc_channel_value[1];	
					index++;
					message[index]	 = rc_channel_value[2]; 
					index++;
					message[index]	 = (int)(RC_in1*DIGITS3);	
					index++;
					message[index]	 = (int)(RC_in2*DIGITS3);	
					index++;
					message[index]	 = (int)(RC_in3*DIGITS3); 
					index++;
					message[index]	 = (int)(RC_thrust*DIGITS3);	
					index++;
					message[index]	 = (int)(RC_angle1*DIGITS3);	
					index++;
					message[index]	 = (int)(RC_angle2*DIGITS3); 
					index++;
					message[index]	 = (int)(ref2_x[REF1_XC1]*DIGITS3); 
					index++;
				}
				#endif
				
				#ifdef SEND_PROVAVOLO
				{
					message[index]	 = (int)(Xpid_err*DIGITS3);	
					index++;
					message[index]	 = (int)(Ypid_err*DIGITS3);	
					index++;
					message[index]	 = (int)(Zpid_err*DIGITS3); 
					index++;
					message[index]	 = rc_channel_value[0];	
					index++;
					message[index]	 = rc_channel_value[1];	
					index++;
					message[index]	 = rc_channel_value[2]; 
					index++;
				}
				#endif
				
				/* 2. send message to ground station */
				unsigned char error_code;
				
				T5CONbits.TON   = 1;
				//error_code = send2gs(data_to_be_send);
				error_code = send_msg(message);

				/* 4. checking errors */
				if(error_code!=0) LED_RED = LED_ON;   
				else  LED_RED = LED_OFF;
			}
			#endif
				
			flag_control_unit_period  = OFF;
			
        }   //end flag_control_unit_period routine 
		
    }   //end infinity loop
       
    return 0;
};


void init(void)
{
    int i;
    
    flag_file_created = OFF;
    
    /**************************************************************************/
    // Inizializza stato della macchina
    machine_state				= SYSTEM_BOOT;
    stateS                      = &state_start;		//-> go to INIT state when TMR1 wake-up
    
    /**************************************************************************/
    // Reset dei buffer di misura, dei filtri e dei relativi puntatori
    for(i=0;i<IMU_BUFFER_LENGTH;i++)
	{
		imu_buf_ax[i] = 0;
		imu_buf_ay[i] = 0;
		imu_buf_az[i] = 0;
		imu_buf_gx[i] = 0;
		imu_buf_gy[i] = 0;
		imu_buf_gz[i] = 0;
	}
	for(i=0;i<EXT_BUFFER_LENGTH;i++)    
	{
		ext_buf_gx[i] = 0;
		ext_buf_gy[i] = 0;
		ext_buf_gz[i] = 0;
	}
    //pointers buffer initialiazation
	pgx 	= ext_buf_gx;
	pgy 	= ext_buf_gy;
	pgz 	= ext_buf_gz;
	pb		= ext_buf_vb;
	ps		= ext_buf_sn;
	gx_IN  	= &bufferIN[15];
	gy_IN  	= &bufferIN[20];
	gz_IN  	= &bufferIN[25];
	gx_OUT 	= &bufferOUT[15];
	gy_OUT 	= &bufferOUT[20];
	gz_OUT 	= &bufferOUT[25];
    
    //butterworth buffers
	for(i=0;i<30;i++)
	{
		bufferIN[i]  = 0.0;
		bufferOUT[i] = 0.0;
	}
    // RC filter buffers 
	RC_thrust_p = 0.0;
	RC_angle1_p = 0.0;
	RC_angle2_p = 0.0;
    for(i=0;i<RC_CHANNELS;i++) rc_error[i] = 0;		 // clear receiver errors
    
    /**************************************************************************/
    // Reset flags e contatori generali
	flag_actuator               = OFF;  //per gestire la riacenzione
    timeout_switch_on           = OFF;  //per gestire la riacenzione
    flag_calibration_request	= OFF;  //all'avvio non viene fatta la calibrazione automatica  
    flag_control_init 			= OFF;
    flag_control                = OFF;
    
    /**************************************************************************/
    // Interrupt nesting enabled
    INTCON1bits.NSTDIS 			= OFF;
    
    /**************************************************************************/
    // Costanti del controllo
   	control1_b      = JX*TCONINV*TCONINV;	// [rad] -> [Nm]
   	control2_b      = JY*TCONINV*TCONINV;	// [rad] -> [Nm]
   	control3_b      = JZ*TCONINV*TCONINV;	// [rad] -> [Nm]
    
    /**************************************************************************/
    // Reset reference generator
    float ref1_x0[REF1_NX] 	= {REF1_XC1_0, REF1_XC2_0, REF1_XC3_0, REF1_XQ1_0, REF1_XQ2_0}; 
	float ref2_x0[REF1_NX] 	= {REF1_XC1_0, REF1_XC2_0, REF1_XC3_0, REF1_XQ1_0, REF1_XQ2_0}; 
    ref_init(ref1_xp, &ref1_u, ref1_x0);
	ref_init(ref2_xp, &ref2_u, ref2_x0);
    
    /**************************************************************************/
    // Initialize communication
    comm_init();    //alloca buffer di trasmissione
    
    /**************************************************************************/
    // Receiver configuration
    rc_config();
    
    /**************************************************************************/
    // PWM configuration
    prop_config();
    
    /**************************************************************************/
    // Peripheral configuration
    module_config();
    
    /**************************************************************************/
    // SPI 1 configuration (disabled because there is the SD card now)
    //config_SPI();    
    
    /**************************************************************************/
    // Iniziallizzo variabili del controllo
    // NO! viene chiamata nello stato INIT
    //control_init();
   
	/**************************************************************************/
    // Accensione perifeiche
    U2MODEbits.UARTEN   = ON;   	//UART2 (reception) enabled
    U2STAbits.UTXEN     = ON;   	//UART2 (transmission) enabled

    //turn-on ADC module
    AD1CON1bits.ADON    = ON; 
    AD2CON1bits.ADON    = ON; 

    MPU6000_conf(); 				//IMU setup and SPI1 turn-on

    rc_init();						//receiver library initialiatation

    prop_start();					//activate PWM modules with minimum duty cycle and wait

    T2CONbits.TON       = ON;   	//start external sensors reading (analog inputs and external gyroscope)

    _INT3IE 			= ON; 		//start IMU reading
	
    /**************************************************************************/
    // System power-up 
	T1CONbits.TON    = ON;          //tick system timer at 50Hz -> the system starts
}

void control_init(void)
{
	int i;
	
    /**************************************************************************/
    // Reset del control step
    control_step   	 = 0;
    
    /*********************************************************/
	//spegnimento attuatori
	prop_off();		
	
	//vertical_thrust  = RAMP_START;  //serve per la rampa
    
    /*********************************************************/
	//reset degli angoli di assetto 
	q0[0] 			 = 1;
    q0[1] 			 = 0;
    q0[2] 			 = 0;
    q0[3] 			 = 0;	
    for(i=0;i<3;i++) attitude[i] = 0;       // angles
	for(i=0;i<4;i++) q_p[i] 	 = q0[i];	// quaternion
	
    /*********************************************************/
    //reset dei comandi
	for(i=0;i<4;i++) ud_p[i] = ud0[i];
	control1_uc_p = 0;		//Tx
	control2_uc_p = 0;		//Ty
	control3_uc_p = 0;		//Tz
	
	/*********************************************************/
    //reset stati del PID
	Xpid_int_p = 0;
	Ypid_int_p = 0;
	Zpid_int_p = 0;
	
    /*********************************************************/
	gs_thrust  = 0;  //non utilizzata
    flag_message_send = 1;  //altrimenti non risponde alla richiesta di stato
    
    
    /*********************************************************/
    //reset stati degli osservatori
	for(i=0;i<N_STATE_OBS1;i++) 					
	{
		obs1_xp[i] = 0;
		obs2_xp[i] = 0;
		obs3_xp[i] = 0;
	}      	
	/*********************************************************/
    //reset della calibrazione
    flag_calibration = OFF;     //se stava calibrando allora la ferma
    media_gyro       = 0;
    for(i=0;i<3;i++)
    {
        media1[i] = 0.0;
        media2[i] = 0.0;
        media3[i] = 0.0;
    }
    /*********************************************************/
    //reset del target
	for(i=0;i<3;i++) target[i] = 0;
       
    /*********************************************************/
	flag_control_init 	= OFF;	
    flag_control        = OFF;
    flag_actuator       = OFF;  //per gestire la riacenzione
    
    #ifdef USE_SD_CARD_LOG
        if(flag_file_created) //se il file è già stato creato allora chiudilo
        {
            // Close the file to save the data
            if (FILEIO_Close (&file) != FILEIO_RESULT_SUCCESS)
            {
                //demoState = DEMO_STATE_FAILED;
            }
            else
            {
                LED_GREEN = LED_ON;
                flag_file_created = OFF;
                crea_file();
            }    
        }
        else
        {
            crea_file();    //la prima volta entra qui
        }
    #endif
 
};

//questa interruzione deve essere modificata. togliere attese attive e gestire tutto con interruzioni
void __attribute__((__interrupt__,__no_auto_psv__)) _T2Interrupt(void)      //frequency call: 400Hz (2.5ms) - duration: 0.12ms (SPTIME=3)
{
    IFS0bits.T2IF = 0;  

	int resto;

	//analog channel reading
	AD1CON1bits.SAMP	= 0;		//start conversion for channel AN16
	AD2CON1bits.SAMP	= 0;		//start conversion for channel AN15	

	while(!AD1CON1bits.DONE) {}		//waiting for AN16 conversion (4.2us)
	//the conversion has finished and a new sampling has already began
	*(ps)		= ADC1BUF0;			//current or sonar

	while(!AD2CON1bits.DONE) {}		//waiting for AN15 conversion (4.2us)
	//the conversion has finished and a new sampling has already began
	*(pb)		= ADC2BUF0;				//battery voltage


//    //gyroscope reading (5 MHz SPI communication)
//    error_gyroX = read_gyro(0, pgx);
//    error_gyroY = read_gyro(1, pgy);
//    error_gyroZ = read_gyro(2, pgz);

      //error checking for gyro SPI communication    
//    if(error_gyroX!=1 || error_gyroY!=1 || error_gyroZ!=1) 
//    {
//        LED_YELLOW = LED_ON;
//    }  
//    else   LED_YELLOW = LED_OFF;
//
	//circular buffer implementation
	resto = pgx - &ext_buf_gx[0];			//NOTE: all buffers have the same length
	
	if(resto==(EXT_BUFFER_LENGTH-1))
	{
		pgx 	= ext_buf_gx;
		pgy 	= ext_buf_gy;
		pgz 	= ext_buf_gz;
		pb		= ext_buf_vb;
		ps		= ext_buf_sn;
	}
	else
	{
		pgx +=1;
		pgy +=1;
		pgz +=1;
		pb  +=1;
		ps  +=1;
	}	
        
};



void __attribute__((__interrupt__,__no_auto_psv__)) _T1Interrupt(void)      //main timer: reading sensor and updating machine state
{   
    IFS0bits.T1IF 		= OFF;  	//1. clear interrupt flag
		
	(*stateS)();					//2. update the state machine				
	
	flag_control_unit_period = ON;	//3. switch-on control unit routine
	   
};

//command decoding function
void gs_decoding(void)
{
    char message[4];
    char command;  
    unsigned int i, old_ipl;
    unsigned char* pchar;
    int val;
   
    
    //prelevo il buffer scritto dalla UART interrupt
    for(i=0;i<4;i++) message[i] = 0;
    SET_AND_SAVE_CPU_IPL(old_ipl,7);
    for(i=0;i<4;i++) 
    {
        message[i] = pc_msg[i];
        pc_msg[i]  = 0;
    }
    RESTORE_CPU_IPL(old_ipl);

    //check del messaggio
    int j=0;
    command=0;
    for(i=0;i<4;i++)
    {
        if(message[i]==-1) j++;
        else command = message[i];
    }
    if(j==3) //comando rilevato nel messaggio
    {
    
//COMMAND DECODING
        switch(command)
        {
            case GS_GET_STATUS:
 
                pchar = (unsigned char*)&val;   
				
                if(flag_message_send)
                {
                    flag_message_send = 0;
                    LED_RED = ~LED_RED;
                    for(i=0;i<N_DATA;i++) data_to_be_send[i] = 0;
                    
                    data_to_be_send[0]  = MSG_BYTE_BEGIN;
                    data_to_be_send[1]  = MSG_BYTE_BEGIN;
                    data_to_be_send[2]  = MSG_BYTE_BEGIN;
                    
                    data_to_be_send[3] = machine_state;
                    
                    //variables to send to the ground station
                    val = (int)(ext_mis_vb*100);
                    data_to_be_send[4] = *(pchar);
                    data_to_be_send[5] = *(pchar+1);
					
                    val = (int)(ext_mis_sn*100);
                    data_to_be_send[6] = *(pchar);
                    data_to_be_send[7] = *(pchar+1);
					
                    val = (int)((RC_thrust + FZ_INIT)*100);
                    data_to_be_send[8] = *(pchar);
                    data_to_be_send[9] = *(pchar+1);
                    
                    data_to_be_send[10] = MSG_BYTE_END;
                    data_to_be_send[11] = MSG_BYTE_END;
                    data_to_be_send[12] = MSG_BYTE_END;

                    //avvio trasmissione
                    send2gs(&data_to_be_send[0]);
                }
                
                break;
            case GS_CALIBRATION_ON:		
                flag_calibration_request = ON;
            break;

            case GS_CONTROL_ON:
                if(machine_state==READY) flag_actuator = ON;
            break;

            case GS_TARGET:
            target[0] = pc_msg[1];
            target[1] = pc_msg[2];
            target[2] = pc_msg[3];
            break;

            case GS_RESET:
                flag_actuator = OFF;
            break;

            case GS_SHUT_DOWN:
                flag_actuator = OFF;
            break;

            case GS_CLOSE_FILE:
                #ifdef USE_SD_CARD_LOG
                    FILEIO_Close (&file);
                    // We're done with this drive.
                    // Unmount it.
                    FILEIO_DriveUnmount ('A');
                #endif
                LED_GREEN 					= LED_OFF;
            break;

            case GS_THRUST_UP:
            gs_thrust += 0.8;
            break;

            case GS_THRUST_DOWN:
            gs_thrust -= 0.8;
            break;

        }
    }
};


void serialize_float(float *data_inn, unsigned char *serial_out, uint8_t n_var)
{
    unsigned int i,kk;
    unsigned char *p = serial_out;
    unsigned char *pp;  //puntatore ai byte interni di un float
    
    for(i=0;i<n_var;i++)   
    {
        kk = 4*i;
        pp = (unsigned char*)&data_inn[i];   //dal LSB fino al MSB
        *(p + kk)     = *pp;
        *(p + kk + 1) = *(pp+1);
        *(p + kk + 2) = *(pp+2);
        *(p + kk + 3) = *(pp+3);
    }
};

void crea_file(void)
{
    FILEIO_SEARCH_RECORD record_file_trovato;
    
    LED_GREEN 					= LED_OFF;
    
    sprintf(file_test_name, "TEST%d.BIN", n_test);

    while(!FILEIO_Find (file_test_name, 0, &record_file_trovato, 1)) 
    {
        n_test++; 
        sprintf(file_test_name, "TEST%d.BIN", n_test);
    }
    
    if (FILEIO_Open (&file, file_test_name, FILEIO_OPEN_WRITE | FILEIO_OPEN_APPEND | FILEIO_OPEN_CREATE) == FILEIO_RESULT_FAILURE)
    {
        //demoState = DEMO_STATE_FAILED;  
        //manda messaggio al PC 
    }
    else
    {
        LED_GREEN 					= LED_ON;
        flag_file_created           = ON;
    }
        
}

void data_packaging(void)
{
    int i;

    //1. measurement
    for(i=0;i<3;i++) *(data_in + i)     = imu_mis_a[i];     //1.1 accelerometer
    for(i=0;i<3;i++) *(data_in + i + 3) = omega[i];         //1.2 gyroscope
    for(i=0;i<3;i++) *(data_in + i + 6) = mag[i];           //1.3 magnetometer
    for(i=0;i<3;i++) *(data_in + i + 9) = gps[i];           //1.4 GPS receiver
    data_in[12]  = ext_mis_vb;                              //1.5 battery
    data_in[13]  = ext_mis_sn;                              //1.6 sonar
    data_in[14]  = 0;                                       //1.7 current
    

    
    //2. reference and operator input
    //2.1 Reference generator 1
    for(i=0;i<5;i++) *(data_in + PACKET_OFFSET_1 + i)       = ref1_x[i];
    data_in[PACKET_OFFSET_1+5] = ref1_u;
    data_in[PACKET_OFFSET_1+6] = cambio_ang;
    //2.2 Reference generator 1
    for(i=0;i<5;i++) *(data_in + PACKET_OFFSET_1 + i + 7)   = ref2_x[i];
    data_in[PACKET_OFFSET_1+12] = ref2_u;
    data_in[PACKET_OFFSET_1+13] = RC_angle1;
    
    
    
    //3. state predictor
    for(i=0;i<6;i++) *(data_in + PACKET_OFFSET_2 + i)       = obs1_xp[i];
    for(i=0;i<6;i++) *(data_in + PACKET_OFFSET_2 + i + 6)   = obs2_xp[i];
    for(i=0;i<6;i++) *(data_in + PACKET_OFFSET_2 + i + 12)  = obs3_xp[i];
    data_in[PACKET_OFFSET_2+18] = obs1_me;
    data_in[PACKET_OFFSET_2+19] = obs2_me;
    data_in[PACKET_OFFSET_2+20] = obs3_me;
    data_in[PACKET_OFFSET_2+21] = obs1_y;
    data_in[PACKET_OFFSET_2+22] = obs2_y;
    data_in[PACKET_OFFSET_2+23] = obs3_y;
    
    
    
    //4. control law
    //4.1 law 1
    for(i=0;i<3;i++) *(data_in + PACKET_OFFSET_3 + i)           = control1_err[i];
    data_in[PACKET_OFFSET_3+3] = control1_fb;
    data_in[PACKET_OFFSET_3+4] = control1_dist;
    data_in[PACKET_OFFSET_3+5] = control1_ref;
    data_in[PACKET_OFFSET_3+6] = control1_uc_p;
    data_in[PACKET_OFFSET_3+7] = uc_emc[TX];            //comando all'impianto
    data_in[PACKET_OFFSET_3+8] = rec_uc[TX];            //comando ricostruito
    data_in[PACKET_OFFSET_3+9] = control1_uc;           //comando ingresso osservatore
    
    //4.2 law 2
    for(i=0;i<3;i++) *(data_in + PACKET_OFFSET_3 + 10 + i)       = control2_err[i];
    data_in[PACKET_OFFSET_3+10+3] = control2_fb;
    data_in[PACKET_OFFSET_3+10+4] = control2_dist;
    data_in[PACKET_OFFSET_3+10+5] = control2_ref;
    data_in[PACKET_OFFSET_3+10+6] = control2_uc_p;
    data_in[PACKET_OFFSET_3+10+7] = uc_emc[TY];        //comando all'impianto
    data_in[PACKET_OFFSET_3+10+8] = rec_uc[TY];        //comando ricostruito
    data_in[PACKET_OFFSET_3+10+9] = control2_uc;       //comando ingresso osservatore
    
    //4.3 law 3
    for(i=0;i<3;i++) *(data_in + PACKET_OFFSET_3 + 20 + i)       = control3_err[i];
    data_in[PACKET_OFFSET_3+20+3] = control3_fb;
    data_in[PACKET_OFFSET_3+20+4] = control3_dist;
    data_in[PACKET_OFFSET_3+20+5] = control3_ref;
    data_in[PACKET_OFFSET_3+20+6] = control3_uc_p;
    data_in[PACKET_OFFSET_3+20+7] = uc_emc[TZ];        //comando all'impianto
    data_in[PACKET_OFFSET_3+20+8] = rec_uc[TZ];        //comando ricostruito
    data_in[PACKET_OFFSET_3+20+9] = control3_uc;       //comando ingresso osservatore
    
    //command
    for(i=0;i<4;i++) *(data_in + PACKET_OFFSET_4 + i)       = uc[i];
    for(i=0;i<4;i++) *(data_in + PACKET_OFFSET_4 + i + 4)   = thrust[i];
    for(i=0;i<4;i++) *(data_in + PACKET_OFFSET_4 + i + 8)   = ud_p[i];
    
    //attitude
    for(i=0;i<3;i++) *(data_in + PACKET_OFFSET_4 + i + 12)  = attitude[i];
    
    //PID
    data_in[PACKET_OFFSET_4 + 15] = Xpid_err;
    data_in[PACKET_OFFSET_4 + 16] = Ypid_err;
    data_in[PACKET_OFFSET_4 + 17] = Zpid_err;
    data_in[PACKET_OFFSET_4 + 18] = Xpid_int;
    data_in[PACKET_OFFSET_4 + 19] = Ypid_int;
    data_in[PACKET_OFFSET_4 + 20] = Zpid_int;
    
    //control step
    data_in[PACKET_OFFSET_4 + 21] = control_step;
    
};