#include "math.h"
#include "SPA_Matrix.h"
#include <stdlib.h>

void QuatMult(float* qa,float* qb,float* qOut,int conj)
{
		int i;
		float qa2[4];
		
		qa2[0]	= qa[0];

		for(i=1;i<4;i++)
		{
			if(conj==1)
				qa2[i]	= -qa[i]; 
			else
				qa2[i]	= qa[i];
		}

    	qOut[0]=qa2[0]*qb[0]-qa2[1]*qb[1]-qa2[2]*qb[2]-qa2[3]*qb[3];	
		qOut[1]=qa2[1]*qb[0]+qa2[0]*qb[1]-qa2[3]*qb[2]+qa2[2]*qb[3];
		qOut[2]=qa2[2]*qb[0]+qa2[3]*qb[1]+qa2[0]*qb[2]-qa2[1]*qb[3];
		qOut[3]=qa2[3]*qb[0]-qa2[2]*qb[1]+qa2[1]*qb[2]+qa2[0]*qb[3];
};

void Quat2Rot(float* q,float** M)
{
	M[0][0]		= q[0]*q[0] + q[1]*q[1] - q[2]*q[2] - q[3]*q[3];
	M[0][1]		= 2*(q[1]*q[2] - q[0]*q[3]);
	M[0][2]		= 2*(q[1]*q[3] + q[0]*q[2]);
	
	M[1][0]		= 2*(q[1]*q[2] + q[0]*q[3]);
	M[1][1]		= q[0]*q[0] - q[1]*q[1] + q[2]*q[2] - q[3]*q[3];
	M[1][2]		= 2*(q[2]*q[3] - q[0]*q[1]);

	M[2][0]		= 2*(q[1]*q[3] - q[0]*q[2]);
	M[2][1]		= 2*(q[2]*q[3] + q[0]*q[1]);
	M[2][2]		= q[0]*q[0] - q[1]*q[1] - q[2]*q[2] + q[3]*q[3];
};

void UpdateQuat(float *q, float *mu1, float *qf, float control_period)
    {
	//////////////UPDATE A DISCRETE TIME QUATERNION////////////
	float vq;
	float c,s;
	float muq[4], qp[4], mu[3];

	int		j;
	
	// remark: mu1 [rad/s]
	for(j=0;j<3;j++) mu[j] = mu1[j]*control_period;
	
	vq		= NormV3(mu);

	c		= cos(0.5*vq);
	
	if(vq > 1e-10)
	{
		s = sin(0.5*vq)/vq;
	}
	else
	{
		s = 0.5;
	}

	muq[0] = 0;
	muq[1] = mu[0];
	muq[2] = mu[1];
	muq[3] = mu[2];
	
	QuatMult(q,muq,qp,0);

	for(j=0;j<4;j++)
		qf[j] = c*q[j] + s*qp[j];

	NormalizeV4(qf);

};

void Euler2rot(const float* eul, float **m, const unsigned int b2f) 
{
    float phi, teta, psi, cteta, cpsi, cphi, sphi,steta,spsi;
    
    phi  	= eul[0];
    teta 	= eul[1];
    psi  	= eul[2];
    
    cteta   = cos(teta);
    cpsi    = cos(psi);
    cphi    = cos(phi);
    
    sphi    = sin(phi);
    steta   = sin(teta);
    spsi    = sin(psi);

    
    if(b2f==0)
    {
        //rotation from fixed to body 3-2-1
        /*
        m[0][0] = cteta*cpsi;
        m[0][1] = -cteta*spsi;
        m[0][2] = steta;
        
        m[1][0] = cphi*spsi + sphi*steta*cpsi;  
        m[1][1] = cphi*cpsi - sphi*steta*spsi;  
        m[1][2] = -sphi*cteta;
        
        m[2][0] = sphi*spsi - cphi*steta*cpsi;  
        m[2][1] = sphi*cpsi + cphi*steta*spsi;  
        m[2][2] = cphi*cteta;
        */
        

        // questo funziona
        //BODY TO FIXED (trasforma un vettore nel corpo nel corrispondente nell'inerziale)
        m[0][0] =  1; //cteta*cpsi;
        m[1][0] =  cteta*spsi;
        m[2][0] = -steta;
        
        m[0][1] = -cphi*spsi + sphi*steta*cpsi;  
        m[1][1] =  1; //cphi*cpsi + sphi*steta*spsi;  
        m[2][1] =  sphi*cteta;
        
        m[0][2] =  sphi*spsi + cphi*steta*cpsi;  
        m[1][2] = -sphi*cpsi + cphi*steta*spsi;  
        m[2][2] =  1; //cphi*cteta;
       
        
    }
    else
    {
        //rotation from body to fixed (3-2-1)trasposta
        /*
        m[0][0] = cteta*cpsi;
        m[1][0] = -cteta*spsi;
        m[2][0] = steta;
        
        m[0][1] = cphi*spsi + sphi*steta*cpsi;  
        m[1][1] = cphi*cpsi - sphi*steta*spsi;  
        m[2][1] = -sphi*cteta;
        
        m[0][2] = sphi*spsi - cphi*steta*cpsi;  
        m[1][2] = sphi*cpsi + cphi*steta*spsi;  
        m[2][2] = cphi*cteta;
        */
        
       
        // funziona
        //FIXED TO BODY
        m[0][0] = cteta*cpsi;
        m[0][1] = cteta*spsi;
        m[0][2] = -steta;
        
        m[1][0] = -cphi*spsi + sphi*steta*cpsi;  
        m[1][1] =  cphi*cpsi + sphi*steta*spsi;  
        m[1][2] =  sphi*cteta;
        
        m[2][0] =  sphi*spsi + cphi*steta*cpsi;  
        m[2][1] = -sphi*cpsi + cphi*steta*spsi;  
        m[2][2] =  cphi*cteta;
    
    }    
    
    /*
    //matrice 3-2-1
    m[0][0] = cteta*cpsi;
    m[1][0] = cteta*spsi;
    m[2][0] = -steta;
    
    m[0][1] = -cphi*spsi + sphi*steta*cpsi;  
    m[1][1] = cphi*cpsi + sphi*steta*spsi;  
    m[2][1] = sphi*cteta;
    
    m[0][2] = sphi*spsi + cphi*steta*cpsi;  
    m[1][2] = -sphi*cpsi + cphi*steta*spsi;  
    m[2][2] = cphi*cteta;
    */
};

void Quat2Euler(float* q, float* angles, unsigned char sequence)
{
    float M11,M33,M12,M13,M23,M21,M31,M32;
    
    //from book formula 1.107
    M11 = q[0]*q[0] + q[1]*q[1] - q[2]*q[2] - q[3]*q[3];
    M12 = 2*(q[1]*q[2]-q[0]*q[3]);
    M13 = 2*(q[1]*q[3]+q[0]*q[2]);
    M21 = 2*(q[1]*q[2]+q[0]*q[3]); 
    M31 = 2*(q[1]*q[3]-q[0]*q[2]);
    M32 = 2*(q[2]*q[3]+q[0]*q[1]); 
    M23 = 2*(q[2]*q[3]-q[0]*q[1]); 
    M33 = q[0]*q[0] - q[1]*q[1] - q[2]*q[2] + q[3]*q[3];   
	
	if(sequence==1)
	{
	    //body to fixed: sequenza 3-2-1
	    angles[0] =  atan2(M32,M33);
	    angles[1] = -asin(M31);
	    angles[2] =  atan2(M21,M11);
	}
	else
	{
	    //body to fixed: sequenza 1-2-3
	    angles[0] = -atan2(M23,M33);
	    angles[1] =  asin(M13);
	    angles[2] = -atan2(M12,M11);
	}
};    

void Body2Inertial(float* q, float* vb, float* vi)
{
    float **M;
	M     = (float**)malloc(12);     
    M[0]  = (float*)malloc(12);
    M[1]  = (float*)malloc(12);
    M[2]  = (float*)malloc(12); 

	Quat2Rot(q,M);   
	DotPMV3(M, vb, vi);
	
}; 

void Rot2Quat(const float** M,float* q)
{
	// input:  M = rotation from body to fixed
	// output: q = quaternion
	
	float	qmax;
	int		k,i;

	//looking for maximum square
	q[0]=(1+M[0][0]+M[1][1]+M[2][2])/4;
	q[1]=(1+M[0][0]-M[1][1]-M[2][2])/4;
	q[2]=(1+M[1][1]-M[0][0]-M[2][2])/4;
	q[3]=(1+M[2][2]-M[0][0]-M[1][1])/4;
	k=0;
	qmax=q[0];
	for (i=1;i<4;i++)
	{
		if(q[i]>qmax)
		{
			k=i;
			qmax=q[i];
		}
	}
	
	if (qmax>1) qmax=1;
	//computing cross products
	switch(k)
	{
		case 0:
			q[0]=sqrt(q[0]);
			q[1]=(M[2][1]-M[1][2])/(4*q[0]);
			q[2]=(M[0][2]-M[2][0])/(4*q[0]);
			q[3]=(M[1][0]-M[0][1])/(4*q[0]);
			break;
		case 1:
			q[1]=sqrt(q[1]);
			q[0]=(M[2][1]-M[1][2])/(4*q[1]);
			q[2]=(M[1][0]+M[0][1])/(4*q[1]);
			q[3]=(M[0][2]+M[2][0])/(4*q[1]);
			break;
		case 2:
			q[2]=sqrt(q[2]);
			q[0]=(M[0][2]-M[2][0])/(4*q[2]);
			q[1]=(M[1][0]+M[0][1])/(4*q[2]);
			q[3]=(M[2][1]+M[1][2])/(4*q[2]);
			break;
		case 3:
			q[3]=sqrt(q[3]);
			q[0]=(M[1][0]-M[0][1])/(4*q[3]);
			q[1]=(M[0][2]+M[2][0])/(4*q[3]);
			q[2]=(M[2][1]+M[1][2])/(4*q[3]);
			break;
	}
};

void UpdateQuatFAST(float *quat,float *ome, float *qp)
{
    // QUATERNION KINEMATICS
    float delta_quat[4], omega[3];
    float y, qnorm;

	omega[0] = ome[0];
	omega[1] = ome[1];    
	omega[2] = ome[2];
	
	//prodotto tra quaternioni
	delta_quat[0] = -omega[0]*quat[1] - omega[1]*quat[2] - omega[2]*quat[3];	
	delta_quat[1] =  omega[0]*quat[0] - omega[1]*quat[3] + omega[2]*quat[2];
	delta_quat[2] =  omega[0]*quat[3] + omega[1]*quat[0] - omega[2]*quat[1];
	delta_quat[3] = -omega[0]*quat[2] + omega[1]*quat[1] + omega[2]*quat[0];
	
	
	qp[0] = quat[0] + 0.5*delta_quat[0];
	qp[1] = quat[1] + 0.5*delta_quat[1];
	qp[2] = quat[2] + 0.5*delta_quat[2];
	qp[3] = quat[3] + 0.5*delta_quat[3];	
			
	y = qp[0]*qp[0]+qp[1]*qp[1]+qp[2]*qp[2]+qp[3]*qp[3];
	
    qnorm = 1/sqrt(y);
    
    qp[0] = qp[0]*qnorm;
    qp[1] = qp[1]*qnorm;
    qp[2] = qp[2]*qnorm;
    qp[3] = qp[3]*qnorm; 
};

void Euler2Quat(const float *euler, float *qout, const unsigned int b2f )
{
	//float temp[3][3]; // temp non è un doppio puntatore!
	// per il doppio puntatore si neccessita malloc()
	
	float **temp;
	temp 	= (float**)malloc(sizeof(float)*3);	//alloco 9 float
	temp[0]	= (float*)malloc(sizeof(float)*3);
	temp[1]	= (float*)malloc(sizeof(float)*3);
	temp[2]	= (float*)malloc(sizeof(float)*3);
	
	Euler2rot(euler, temp, b2f);
	Rot2Quat((const float**)temp, qout);
	
	free(temp[0]);
	free(temp[1]);
	free(temp[2]);
	free(temp);
};

/*
extern void		Rot2Quat(float** M,float* q)
{
	// input:  M = rotation from body to fixed
	// output: q = quaternion
	
	float	qmax;
	int		k,i;

	//looking for maximum square
	q[0]=(1+M[0][0]+M[1][1]+M[2][2])/4;
	q[1]=(1+M[0][0]-M[1][1]-M[2][2])/4;
	q[2]=(1+M[1][1]-M[0][0]-M[2][2])/4;
	q[3]=(1+M[2][2]-M[0][0]-M[1][1])/4;
	k=0;
	qmax=q[0];
	for (i=1;i<4;i++)
	{
		if(q[i]>qmax)
		{
			k=i;
			qmax=q[i];
		}
	}
	
	if (qmax>1) qmax=1;
	//computing cross products
	switch(k)
	{
		case 0:
			q[0]=sqrt(q[0]);
			q[1]=(M[2][1]-M[1][2])/(4*q[0]);
			q[2]=(M[0][2]-M[2][0])/(4*q[0]);
			q[3]=(M[1][0]-M[0][1])/(4*q[0]);
			break;
		case 1:
			q[1]=sqrt(q[1]);
			q[0]=(M[2][1]-M[1][2])/(4*q[1]);
			q[2]=(M[1][0]+M[0][1])/(4*q[1]);
			q[3]=(M[0][2]+M[2][0])/(4*q[1]);
			break;
		case 2:
			q[2]=sqrt(q[2]);
			q[0]=(M[0][2]-M[2][0])/(4*q[2]);
			q[1]=(M[1][0]+M[0][1])/(4*q[2]);
			q[3]=(M[2][1]+M[1][2])/(4*q[2]);
			break;
		case 3:
			q[3]=sqrt(q[3]);
			q[0]=(M[1][0]-M[0][1])/(4*q[3]);
			q[1]=(M[0][2]+M[2][0])/(4*q[3]);
			q[2]=(M[2][1]+M[1][2])/(4*q[3]);
			break;
	}
}
*/

