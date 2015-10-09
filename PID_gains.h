// PID GAIN LIBRARY

/**** FOR Y AXIS ****/
//con gamma in 0.2 0.2 0.2 è instabile
//con gamma in 0.2 0.2 0.1 è al limite della stabilità



//prova: gamma 0.1 0.1 0.1

#define Kp  2.6
#define Kd  0.36
#define Ki  6


/*** Ixx ***/
#define B_INV_X   11.858075

/*** Iyy ***/   
#define B_INV_Y   11.853725


/**** INERTIA  ****/

#define Ixx_inv 210.6505
#define Iyy_inv 210.9041
#define Izz_inv 105.4415

/**** MOTORS COEFFICIENTS ****/

// Motor 1
#define C2_0 	 -0.001630
#define C1_0 	  0.049606
#define C0_0 	  0.163012
#define Cbatt_0   -0.014727 



// Motor 3
#define C2_2 	  -0.001902
#define C1_2 	   0.060639
#define C0_2 	  0.153985
#define Cbatt_2	 -0.007757





