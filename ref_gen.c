#include "ref_gen.h"

// variabili interne
int i;	

void ref_init(float* xp, float* uref, float* x0)	
{
	for(i=0;i<REF1_NX;i++)
	{
		xp[i] = x0[i];
	}
	*(uref) = 0.0;
};

void ref_update(const float* xp, float* x)
{
	for(i=0;i<REF1_NX;i++)
	{
		x[i] = xp[i];
	}
};

void ref_gen(const float* x, const float* input, float* xp, float* u)		
{
	float e;
	e				= *(input) - x[REF1_XC1];					//errore di modello [rad]
	
	/*
	xp[REF1_XC1]	= x[REF1_XC1] + x[REF1_XC2];			//
	xp[REF1_XC2]	= x[REF1_XC2] + x[REF1_XC3];			//
	xp[REF1_XC3]	= x[REF1_XC3]*(1-REF1_BD) + x[REF1_XQ1]*REF1_L1 + x[REF1_XQ2]*REF1_L2 + REF1_L0*e;	//
	xp[REF1_XQ1]	= x[REF1_XQ1]*(1-REF1_Q1) + x[REF1_XQ2];
	xp[REF1_XQ2]	= x[REF1_XQ2] - x[REF1_XQ1]*REF1_Q2 + e;
	*(u)			= (x[REF1_XQ1]*REF1_L1 + x[REF1_XQ2]*REF1_L2 + REF1_L0*e)*REF1_BD_INV;
	*/
	
	
	//modificato: bd in ingresso
	xp[REF1_XC1]	= x[REF1_XC1] + x[REF1_XC2];			//
	xp[REF1_XC2]	= x[REF1_XC2] + x[REF1_XC3];			//
	xp[REF1_XC3]	= x[REF1_XC3]*(1-REF1_BD) + (x[REF1_XQ1]*REF1_L1 + x[REF1_XQ2]*REF1_L2 + REF1_L0*e)*REF1_BD;	//
	xp[REF1_XQ1]	= x[REF1_XQ1]*(1-REF1_Q1) + x[REF1_XQ2];
	xp[REF1_XQ2]	= x[REF1_XQ2] - x[REF1_XQ1]*REF1_Q2 + e;
	*(u)			= x[REF1_XQ1]*REF1_L1 + x[REF1_XQ2]*REF1_L2 + REF1_L0*e;
	
};
