#include "SPA_Matrix.h"
#include "MemHandling.h"
#include "math.h"
#include "quadcopter_borea.h"

extern void		InvertMat3(float** x,float** invx)
{
	float	det;

	det			= x[0][0]*(x[1][1]*x[2][2]-x[2][1]*x[1][2])-x[0][1]*(x[1][0]*x[2][2]-x[1][2]*x[2][0])+x[0][2]*(x[1][0]*x[2][1]-x[1][1]*x[2][0]);

	invx[0][0]	= (x[1][1]*x[2][2]-x[2][1]*x[1][2])/det;
	invx[0][1]	= -(x[1][0]*x[2][2]-x[1][2]*x[2][0])/det;
	invx[0][2]	= (x[1][0]*x[2][1]-x[2][0]*x[1][1])/det;

	invx[1][0]	= -(x[0][1]*x[2][2]-x[0][2]*x[2][1])/det;
	invx[1][1]	= (x[0][0]*x[2][2]-x[0][2]*x[2][0])/det;
	invx[1][2]	= -(x[0][0]*x[2][1]-x[2][0]*x[0][1])/det;

	invx[2][0]	= (x[0][1]*x[1][2]-x[0][2]*x[1][1])/det;
	invx[2][1]	= -(x[0][0]*x[1][2]-x[1][0]*x[0][2])/det;
	invx[2][2]	= (x[0][0]*x[1][1]-x[1][0]*x[0][1])/det;
}

extern void		MultVByScl(float* x,float a,float* y,int N)
{
	int		i;

	for(i=0;i<N;i++)
	{
		y[i] = a*x[i];
	}	
}

extern void		MultMByScl(float** x,float a,float** y,int Nr,int Nc)
{
	int		i,j;

	for(i=0;i<Nr;i++)
	{
		for(j=0;j<Nc;j++)
		{
			y[i][j] = a*x[i][j];
		}	
	}
}

extern void		MultVByScl3(float* x,float a,float* y)
{
	int		i;

	for(i=0;i<3;i++)
	{
		y[i] = a*x[i];
	}	
}

extern void		MultMByScl3(float** x,float a,float** y)
{
	int		i,j;

	for(i=0;i<3;i++)
	{
		for(j=0;j<3;j++)
		{
			y[i][j] = a*x[i][j];
		}	
	}
}

extern void		AddV(float* x1,float* x2,float* y,int N)
{
	int		i;

	for(i=0;i<N;i++)
	{
		y[i] = x1[i] + x2[i];
	}	
}

extern void		AddV3(float* x1,float* x2,float* y)
{
	int		i;

	for(i=0;i<3;i++)
	{
		y[i] = x1[i] + x2[i];
	}	
}


extern float	SumVEl(float* x,int N)
{
	int		i;
	float	sum = 0;

	for(i=0;i<N;i++)
	{
		sum += x[i];
	}

	return sum;
}

extern float	SumMColEl(float** x,int N,int k)
{
	int		i;
	float	sum = 0;

	for(i=0;i<N;i++)
	{
		sum += x[i][k];
	}

	return sum;
}

extern void		DotPMV(float** m,float* x,float* dotP, int Nr, int Nc)
{
	int		i,j;
	
	for(i=0;i<Nr;i++)
	{
		for(j=0;j<Nc;j++)
		{
			dotP[i] += m[i][j]*x[j];
		}
	}
}

extern void		DotPMV3(float** m,float* x,float* dotP)
{
	int		i,j;
	
	for(i=0;i<3;i++)
	{
    	dotP[i] = 0;
		for(j=0;j<3;j++)
		{
			dotP[i] += m[i][j]*x[j];
		}
	}
}

void DotPMM(float **x1, float **x2, float** dotP, int Nr1, int Nc1, int Nr2, int Nc2)
{
	int		i,j,k;
	
	for(i=0;i<Nr1;i++)
	{
		for(j=0;j<Nc2;j++)
		{
    		dotP[i][j] = 0;     //modifica
			for(k=0;k<Nc1;k++)
			{
				dotP[i][j] += x1[i][k]*x2[k][j];
			}
		}	
	}
}

extern void		DotPMM3(float** x1,float** x2,float** dotP)
{
	int		i,j,k;
	
	for(i=0;i<3;i++)
	{
		for(j=0;j<3;j++)
		{
			for(k=0;k<3;k++)
			{
				dotP[i][j] += x1[i][k]*x2[k][j];
			}
		}	
	}
}

extern void		AddM(float** x1,float** x2,float** y,int Nr,int Nc)
{
	int		i,j;

	for(i=0;i<Nr;i++)
	{
		for(j=0;j<Nc;j++)
		{
			y[i][j] = x1[i][j] + x2[i][j];
		}	
	}
}

extern void		AddM3(float** x1,float** x2,float** y)
{
	int		i,j;

	for(i=0;i<3;i++)
	{
		for(j=0;j<3;j++)
		{
			y[i][j] = x1[i][j] + x2[i][j];
		}	
	}
}

/*
extern void		SaveMatIntoDatFile(float** m,int Nr,int Nc,char* fileName)
{
	FILE*	myFile;
	float	toFile;

	int		i,j;

	myFile = fopen( fileName, "wb" );

	toFile = Nr;
	fwrite(&toFile,8,1,myFile);
	
	toFile = Nc;
	fwrite(&toFile,8,1,myFile);

	for(i=0;i<Nr;i++)
	{
		for(j=0;j<Nc;j++)
		{
			toFile = m[i][j];
			fwrite(&toFile,8,1,myFile);
		}
	}

	fclose(myFile);
}
*/

extern float	Sat(float x,float min, float max)
{
	float y;

	y = x;

	if(x >= max)
	{
		y = max;
	}

	if(x <= min)
	{
		y = min;
	}

	return y;
}

extern float	Abs(float x)
{
	float y;

	if(x >= 0)
	{
		y = x;
	}

	if(x < 0)
	{
		y = -x;
	}

	return y;
}

extern float	Max(float *x, int N)
{
	float	currMax;
	int		i;

	currMax = x[0];

	for(i=0;i<N-1;i++)
	{
		if(x[i+1]>currMax)
		{
			currMax = x[i+1];
		}
	}

	return currMax;
}
	
extern float	Min(float *x, int N)
{
	float	currMin;
	int		i;

	currMin = x[0];

	for(i=0;i<N-1;i++)
	{
		if(x[i+1]<currMin)
		{
			currMin = x[i+1];
		}
	}

	return currMin;
}

extern float	Mean(float *x, int N)
{
	float	sum;
	int		i;

	sum		= 0;

	for(i=0;i<N;i++)
	{
		sum += x[i];
	}

	return (1/(float)N)*sum;
}

extern float	NormV3(float *x)
{
	float	y;
	int		i;

	y = 0;

	for(i=0;i<3;i++)
	{
		y += x[i]*x[i];
	}

	return sqrt(y);
}

extern float	NormV4(float *x)
{
	float	y;
	int		i;

	y = 0;

	for(i=0;i<4;i++)
	{
		y += x[i]*x[i];
	}

	return sqrt(y);
}

extern void		NormalizeV4(float *q)
{
	float	qnorm;
	int		i;

	qnorm = NormV4(q);
	
	for(i=0;i<4;i++)
		q[i] = q[i]/qnorm;
}


#define         fFaddeev   "polcar - Fine "
#define         eDvector   "polcar - Errore allocazione memoria vettore DP"
#define         eDmatrix   "polcar - Errore allocazione memoria matrice DP"

/*
extern float *polcar(float **A,long int n)
{
	long int i,k,h,j,ir;
	float *c;
	float **AM,**BM,**CM;
	float e;
	char *ch=NULL;


	if((AM=dmatrix(n,n,0.))==NULL) ch=eDmatrix;

	if((BM=dmatrix(n,n,0.))==NULL) ch=eDmatrix;

	if((CM=dmatrix(n,n,0.))==NULL) ch=eDmatrix;

	if((c=dvector(n+1,0.))==NULL) ch=eDvector;


	for(i=0;i<n;i++) BM[i][i]=1.; 

	//matrice quasi diagonale ma reale 
	for(i=0;i<n;i++)
	if(A[i][1]==0.)  AM[i][i]=1.-A[i][0];
	else 
	{
	AM[i][i]=1.;
	AM[i][i+1]=-1.;
	AM[i+1][i+1]=1.-2*A[i][0];  // doppio PR 
	AM[i+1][i]=A[i][0]*A[i][0]+A[i][1]*A[i][1]; //  PR*PR+PI*PI 
	i++; 
	}

	for(i=0;i<n;i++)
	{		
		ir=n-1-i;
		for(k=0;k<n;k++) for (h=0;h<n;h++) 
		{
		CM[k][h]=0.;
		for(j=0;j<n;j++) CM[k][h]=CM[k][h]+AM[k][j]*BM[j][h];  
		}

		for(k=0;k<n;k++) c[ir]=c[ir]+CM[k][k];  // traccia 
		c[ir]=c[ir]/(i+1); 

		for(k=0;k<n;k++) for (h=0;h<n;h++)
		{
		BM[k][h]=-CM[k][h]; 
		if(k==h) BM[k][h]=BM[k][h]+c[ir];
		}
		

	}

	e=0;		
	for(k=0;k<n;k++) 
	for(h=0;h<n;h++) e=e+fabs(BM[k][h]);
	c[n]=e/(n*n); 

	 

	MyFree(*AM);
	MyFree(AM);
	MyFree(*BM);
	MyFree(BM);
	MyFree(*CM);
	MyFree(CM);
	if (ch==NULL) ch=fFaddeev;
	
	return(c);
}

*/
