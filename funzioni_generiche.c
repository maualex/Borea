#include "quadcopter_borea.h"

float plane(propeller,uforce,Vbatt)
{
}

void propellers(float* uforce)
{
    unsigned char propeller;
    
    // _________SATURATIONS ( +10N : +1.5N )
    for(propeller=0;propeller<4;propeller++)
    {
        //saturazioni
        if(uforce[propeller]>THRUST_SAT_U) uforce[propeller] = THRUST_SAT_U;
        if(uforce[propeller]<THRUST_SAT_D) uforce[propeller] = THRUST_SAT_D;
        
        //trasformo la forza in duty cycle tra 0 e 1 (piano caratteristico)
        // duty[propeller] = plane(propeller,uforce,Vbatt);
        
        // passo da duty cycle al valore intero corrispondente
        // attuatore[propeller] = MOTORE_MIN + duty[propeller]*MOTORE_DELTA;     //MOTORE_MIN = MOTORE_DELTA = 625     
    }

    MOTORE1 = floor( CURVE1_2*uforce[0]*uforce[0] + CURVE1_1*uforce[0] + CURVE1_0 ) + MOTORE_MIN + OFFSET1 ;
    MOTORE2 = floor( CURVE2_2*uforce[1]*uforce[1] + CURVE2_1*uforce[1] + CURVE2_0 ) + MOTORE_MIN + OFFSET2 ;
    MOTORE3 = floor( CURVE3_2*uforce[2]*uforce[2] + CURVE3_1*uforce[2] + CURVE3_0 ) + MOTORE_MIN + OFFSET3 ;
    MOTORE4 = floor( CURVE4_2*uforce[3]*uforce[3] + CURVE4_1*uforce[3] + CURVE4_0 ) + MOTORE_MIN + OFFSET4 ;              
}

void propellers_off()
{
            
            MOTORE1 = MOTORE_MIN;
        
            MOTORE2 = MOTORE_MIN; 
        
            MOTORE3 = MOTORE_MIN;    
        
            MOTORE4 = MOTORE_MIN;  
};


void propellers_equal_force(float f)
{
        float force[4] = {f,f,f,f};
        propellers(force);        
};