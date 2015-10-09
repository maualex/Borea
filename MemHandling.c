#include "MemHandling.h"
#include <stdlib.h>
#include <stdio.h>

void SaveToFile_data(double *data1, char *FileName,long int long_data,float delta)
{
	double 	temp,sTime;
	int i;
	float  tofile;
	
	double *aux2;   	
					  
	FILE	*myFile;
	
	sTime	=   delta;
	temp	=	0;
	
	if((myFile=fopen( FileName, "wb" ))==NULL)
	{
		i=0;
	}
	aux2=&data1[0];
	tofile	= (float)2;
	fwrite (&tofile,4,1, myFile ); 
	for (i=0;i<1;i++)
	{
		tofile	= (float)i;
		fwrite (&tofile,4,1, myFile ); 
	}
	
	for (i=0;i<long_data;i++)
	{
		temp	+=	sTime;	
		tofile	=	(float)(temp - sTime);
		fwrite (&tofile,4,1, myFile );
		tofile	=	(float)*aux2;   
		fwrite (&tofile,4,1, myFile );
		aux2++;
	}	
	if(fclose(myFile))
		i=0;
}

double **dmatrix(int dim_i,int dim_j,double dinit)
{
	int        i;
	double          *a,**b;
	//  fare una struttura  28 dic 2007

	if((a=(double*)malloc(sizeof(double) *dim_i*(int)dim_j))==NULL)
		return (NULL);

	/* a  puntatore all'area della matrice */

	if((b=(double**)malloc(sizeof(double*) *dim_i))==NULL)
		return (NULL);

	/* puntatore ai puntatori dell'inizio di ogni riga -
	   ricorda che in C le matrici sono registrate
	   riga per riga (il primo vettore e'la prima riga) */

	for (i=0;i<(dim_i*dim_j);i++)
		*(a+i)=dinit;

	for (i=0;i<dim_i;i++)
		*(b+i)=a+i*dim_j;

	return(b);
}

double * dvector(int dim, double dinit)
{
	int        i;
	double     *a;

	if((a=(double*)malloc(sizeof(double) *dim))==NULL)
		return (NULL);

	/* a  puntatore all'area del vettore*/

	for (i=0;i<dim;i++)
		*(a+i)=dinit;

	return(a);
};  

float * fvector(int dim, float dinit)
{
	int        i;
	float     *a;

	if((a=(float*)malloc(sizeof(float) *dim))==NULL)
		return (NULL);

	/* a  puntatore all'area del vettore*/

	for (i=0;i<dim;i++)
		*(a+i)=dinit;

	return(a);
};



long int * ivector(int dim, int dinit)
{
	int     i;
	long int		*a;

	if((a=(long int*)malloc(sizeof(int) *dim))==NULL)
		return (NULL);

	/* a  puntatore all'area del vettore*/

	for (i=0;i<dim;i++)
		*(a+i)=dinit;

	return(a);
}  

extern void MyFree(void *a)
{
	free(a);
}     
