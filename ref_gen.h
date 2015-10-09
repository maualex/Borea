// - -  reference generator  - -
// complementary eigenvalues = [0.240964, 0.224827, 0.209771, 0.195723, 0.182616]
// gains
/*
#define REF1_BD 0.240964
#define REF1_L0 0.033377
#define REF1_L1 0.006300
#define REF1_L2 -0.017405
#define REF1_Q1 0.812938
#define REF1_Q2 0.247331
*/



////piccola modifica: ho inserito bd sull'ingresso
//#define REF1_BD 0.240964
//#define REF1_L0 0.232254
//#define REF1_L1 0.070799
//#define REF1_L2 -0.153903
//#define REF1_Q1 0.963855
//#define REF1_Q2 0.348381



// complementary eigenvalues = [0.500000, 0.125000, 0.031250, 0.007813, 0.001953]
// gains
#define REF1_BD 0.500000
#define REF1_L0 0.000081
#define REF1_L1 -0.000005
#define REF1_L2 0.000027
#define REF1_Q1 0.166016
#define REF1_Q2 0.005447

//#define REF1_BD_INV 4.15

// states
#define REF1_NX  5			// number of states
#define REF1_XC1 0		
#define REF1_XC2 1			
#define REF1_XC3 2
#define REF1_XQ1 3
#define REF1_XQ2 4
// initial states	-> si potrebbero togliere da qui e lasciarli all'applicazione principale
#define REF1_XC1_0 0		
#define REF1_XC2_0 0			
#define REF1_XC3_0 0
#define REF1_XQ1_0 0
#define REF1_XQ2_0 0

//prototypes
void ref_init(float* xp, float* uref, float* x0);
void ref_update(const float* xp, float* x);
void ref_gen(const float* x, const float* input, float* xp, float* u);
