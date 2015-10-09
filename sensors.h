
//ADC
#define RHO_ADC         805.664e-6      // quantizzazione ADC


// IMU (marco fontana)
#define GYRO_RANGE 500          // POSSIBLE VALUES : 250,500,1000,2000;
#define Gyro_Scale_Factor_INV 0.0152671755725191//0.007633587786260//

#define Acc_Scale_Factor 16384
#define Acc_Scale_Factor_INV 0.00006103515625




// BEGIN (mauricio modifications 15/05/2015)

//buffer dimensions
#define IMU_BUFFER_LENGTH		20
#define EXT_BUFFER_LENGTH 		8				//buffer dimension for external measurements 

//IMU ACCELEROMETER PARAMETERS 
#define IMU_ACC_SCALE 			5.9875e-04
//axis
#define IMU_ACC_AXIS_X			1
#define IMU_ACC_AXIS_Y			0
#define IMU_ACC_AXIS_Z			2
//sign
#define IMU_ACC_SIGN_X			+
#define IMU_ACC_SIGN_Y			+
#define IMU_ACC_SIGN_Z			+
//bias
#define IMU_ACC_BIAS_X			-0.0407
#define IMU_ACC_BIAS_Y			+0.2532
#define IMU_ACC_BIAS_Z			+0.7973

//IMU GYROSCOPE PARAMETERS 
#define IMU_GYR_SCALE 			2.6775e-04
//axis
#define IMU_GYR_AXIS_X			1
#define IMU_GYR_AXIS_Y			0
#define IMU_GYR_AXIS_Z			2
//sign
#define IMU_GYR_SIGN_X			+
#define IMU_GYR_SIGN_Y			-
#define IMU_GYR_SIGN_Z			+


// EXTERNAL GYROSCOPE PARAMETERS
#define EXT_GYR_SCALE  2.18166e-004
//axis
#define EXT_GYR_AXIS_X			1
#define EXT_GYR_AXIS_Y			0
#define EXT_GYR_AXIS_Z			2
//sign
#define EXT_GYR_SIGN_X			-
#define EXT_GYR_SIGN_Y			+
#define EXT_GYR_SIGN_Z			-

#define ADC_12BIT_CONVERTION 8.0586e-04     //da digitale a tensione

// EXTERNAL BATTERY
#define EXT_BAT_SCALE			0.00356 //valore trovato da marco sul partitore
#define EXT_BAT_SCALE_CORRECTION    1.018     //valore trovato con una misura
#define EXT_BAT_OFFSET			0.7

// EXTERNAL SONAR
//tensione minima rilevata = 0.04V (le misure sono precise fino alla seconda cifra decimale)
//tensione di alimentazione rilevata Vcc = 3.32V
// S=3.32/2048=0.0016 V/cm (inverso S^-1= 625 cm/V = 6.25 m/V)
//minima distanza rilevabile 25cm -> Vout = S*25cm=0.04V -> si trova!
#define EXT_SON_MAX             10.68       //massima altitudine [m]
#define EXT_SON_MIN              0.25       //minima altitudine [m]
#define EXT_SON_SCALE            6.25       //inverse sensitivity [m/V]
#define EXT_SON_OFFSET           0.003      //3mV di offset rilevato con multimetro
// END (mauricio modifications 15/05/2015)



/* MAGNETOMETER */
#define MAG_BIAS_X 448          // bias magnetometro [LSB]
#define MAG_BIAS_Y 167          // bias magnetometro [LSB]
#define MAG_BIAS_Z -52          // bias magnetometro [LSB]

    //orientation 
#define MAG_X_AXIS  0
#define MAG_Y_AXIS  1
#define MAG_Z_AXIS  2
#define MAG_X_SIGN  +
#define MAG_Y_SIGN  +
#define MAG_Z_SIGN  +


/* ULTRASOUND */
#define SONAR_BIAS -0.06        // bias ultrasuono [m]
#define KSONAR_INV  6.25         // sensitività inversa [m/V]
#define SONAR_SAT_MIN 0.3       //minimum distance [m]

#define SAMPLING_TIME 10
#define CHANNEL AD1CHS0bits.CH0SA
#define AX 11  //12
#define AY 10
#define AZ 9 //10
    
#define AN15 0      //battery connection
#define AN17 17

//GPS receiver


//global variables
//extern double accV[3], gyrV[3];
