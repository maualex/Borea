
#define THRUST_RAMP             ON
//#define DATA_TRASMISSION      
#define IMU 					
#define OPEN_LOOP		
#define ACTUATORS
//#define GRAVITY_VECTOR					//determine the initial attitude from gravity vector
#define CALIBRATION_COUNTER     400		//samples to be average in the calibration phase

//PID GAINS
// gamma = [0.24 0.1 0.01] for X and Y axis
// gamma = [0.1 0.01 0.01] for Z axis
#define PID_KP_X        2.2004
#define PID_KI_X        0.9637
#define PID_KD_X        0.5622

#define PID_KP_Y        2.2484
#define PID_KI_Y        0.9847
#define PID_KD_Y        0.5744

#define PID_KP_Z        0.3230
#define PID_KI_Z        0.0769
#define PID_KD_Z        0.3691


//THRUST RAMP PARAMETERS
#define RAMP_START    6.00000    		//[N]
#define RAMP_END      19.600000			//[N]
#define RAMP_SLOPE    0.060000			//[N/Tcon]

//VERTICAL GUIDANCE PARAMETERS
#define AMAX          0.000800			// [m]
#define VMAX          0.040000			// [m]
#define RHOY          0.100000			// []
#define DELTA         0.400000			// [m]
#define WAIT          100				// [Tcon]

//ATTITUDE CONTROL
//gamma = [0.300000 0.300000 0.300000]
#define K_1           0.027000			// attitude feedback gain
#define K_2           0.270000			// angular rate feedback gain
#define K_3           0.900000			// delay feedback gain

//ATTITUDE OBSERVER
//gamma = [0.600000 0.400000]
#define Lg0           1.000000			//
#define Lg1           0.240000			//

//VERTICAL CONTROL
//gamma = [0.100000 0.100000]
#define KV_VERT       0.200000			// velocity feedback gain
#define KP_VERT       0.010000			// position feedback gain

//VERTICAL OBSERVER
//gamma = [0.050000 0.050000 0.100000 0.100000]
#define MZ1           -0.001652			//
#define MZ2           0.000001			//
#define LZ1           0.006500			//
#define BETAZ         0.300000			//
#define NZ            5					//
#define PZ0           0.000000			//
#define VZ0           0.000000			//
#define DZ0           0.000000			//
#define QZ0           0.000000			//


//FRAME PARAMETERS
#define MCON          2.0000			// quadrotor mass [Kg]
#define MCONINV       0.5000			// 1/quadrotor mass
#define TCON          0.0200			// control step [s]
#define TCONINV       50.000			// control frequency [Hz]
#define TCONHALF      0.0100			// control step/2 [s]
#define GCON         -9.8000			// gravity value in Turin [m/s^2]
#define JX            0.0321			// [Kgm^2]
#define JY            0.0328			// [Kgm^2]
#define JZ            0.0615			// [Kgm^2]
#define JXINV         31.1526			// [Kgm^-2]
#define JYINV         30.4878			// [Kgm^-2]
#define JZINV         16.2602			// [Kgm^-2]
#define ARM_LONG      0.27000			// [m]
#define ARM_LONG_I 	  3.7037
#define KT			  0.02				//modello coppia di reazione motore [Nm/N]
#define KT_I		  50
 

