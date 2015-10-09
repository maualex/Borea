
#define ESC_WAIT 		1000		//[ms]
#define FZ_INIT	    	12    		//[N]

//DIGITAL COMMAND
#define UD_DELTA      	5000			//for 400Hz ESC
#define UD_DELTA_I     	2e-4			//for 400Hz ESC
#define UD_OFFSET     	5000			//for 400Hz ESC

//COMMAND SATURATIONS
#define F_MAX          	8.0			// maximum thrust[N] (for single propeller)
#define F_MIN          	2.0			// minimum thrust[N] (for single propeller)

//#define UC_FZ_MAX		(4*(F_MAX-2*UC_TX_MAX)
#define UC_TX_MAX		0.5			// max command torque [Nm]
#define UC_TY_MAX		0.5			// max command torque [Nm]
#define UC_TZ_MAX		0.1			// max command torque [Nm]

//propeller coefficient recovered by Illinois data
#define PROP_COEF_1 	-2.75
#define PROP_COEF_2 	 82.22 
#define PROP_COEF_3 	 219.44

//ESC coefficient -> from duty cycle to omega
#define ESC_COEF_1_I	-1.7825e4 	
#define ESC_COEF_2_I 	 2.2638e4
#define ESC_COEF_3_I 	-7.9237e3
#define ESC_COEF_4_I 	 4.2203e1
#define ESC_COEF_5_I	 6.8756e2

//propeller coefficient -> from duty cycle to omega
#define PROP_COEF_1_I	 2.0769e-5	 	
#define PROP_COEF_2_I 	-3.5668e-3
#define PROP_COEF_3_I 	 4.2981e-1

//ESC linear model (from angular rate to duty cycle)
#define ESC_COEF_1 		 2.97e-09   
#define ESC_COEF_2 	    -4.74e-06
#define ESC_COEF_3 		 3.13e-03
#define ESC_COEF_4      -2.96e-02

//PWM MODULES
#define ESC_1           OC2RS
#define ESC_2           OC3RS
#define ESC_3           OC4RS
#define ESC_4           OC5RS

//propellers
#define P1				0			
#define P2				1
#define P3				2
#define P4				3
		
extern unsigned char flag_actuator;

void prop_set(const unsigned int* duty);
void prop_off(void);
void prop_config(void);
void prop_start(void);
