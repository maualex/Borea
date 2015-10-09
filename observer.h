/*________________________________________________________________*/
/*___________________________OBSERVERS GAIN_______________________*/

//1. GYROSCOPE OBSERVER (lamda=0.9 -> molto lento)
#define Lg0 0.3 
#define Lg1 0.03 
#define Lg2 0.0009999 

#define Lm0  0.3
#define Lm1  0.03
#define Lm2  0.00099

//2. VERTICAL OBSERVER
#define MZ1     0.7
#define MZ2     0.7
#define LZ1     0.7
#define BETAZ   0.8
