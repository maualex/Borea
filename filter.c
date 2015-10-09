#include "filter.h"

double filter_form1(double x,double *num,double *den, int order,double *xbuffer,double *ybuffer)
{
	double y,a;
	double* temp = num;         //puntatore al primo coeff del num
	double* yout = ybuffer;     //puntatore alla testa del buffer
	int i;
	
	y = 0;
	a = *num;   //salvo il primo coeff per poi rpristinarlo alla fine
	*num=0;
	
	rotate_buffer(0,ybuffer,order+1);
	rotate_buffer(x,xbuffer,order+1); 
	for(i=0;i<=order;i++,den++,num++,xbuffer++,ybuffer++)
	{
		y=y+(*den)*(*xbuffer)-(*num)*(*ybuffer);
	}
	
	*temp = a;  //ripristino il valore del primo coeff del num
	
     y=y/a;      //divisione per il primo coeff del num   	
	
	*yout = y;  //metto il risultato in testa al buffer di uscita
	
	
	
	return y;
}

double filter_form1_FAST(double x,double *num,double *den, int order,double *xbuffer,double *ybuffer)
{
	double y,a;
	double* temp = num;         //puntatore al primo coeff del num
	double* yout = ybuffer;     //puntatore alla testa del buffer
	int i;
	
	y = 0;
	a = *num;   //salvo il primo coeff per poi rpristinarlo alla fine
	*num=0;
	
	for(i=order;i>0;i--)
	{
    	ybuffer[i] = ybuffer[i-1];
    	xbuffer[i] = xbuffer[i-1];
	} 
	    
	ybuffer[0] = 0;
	xbuffer[0] = x;

	for(i=0;i<=order;i++,den++,num++,xbuffer++,ybuffer++)
	{
		y=y+(*den)*(*xbuffer)-(*num)*(*ybuffer);
	}
	
	*temp = a;  //ripristino il valore del primo coeff del num
	
	if(a!=1) { y=y/a; }      //divisione per il primo coeff del num 	
	
	*yout = y;  //metto il risultato in testa al buffer di uscita

	return y;
}

void rotate_buffer(double y,double *buffer,int size)
{
/*	double *buffer_d=buffer+size-1,*buffer_aux=buffer+size-2;
	buffer_d=buffer_d;
	
	for(;buffer_d!=buffer;buffer_d--,buffer_aux--)
		*buffer_d=*buffer_aux;
	*buffer=y;   */
	int i;
	for(i=size-1;i>0;i--)
		buffer[i]=buffer[i-1];
	buffer[0]=y;
	
}

double filter_step(double *data,int step,double *xbuffer,double *ybuffer,double *num,double *den,int order)
{
	double y=0,temp;
	int i;
	for(i=0;i<step;i++)
	{
		
		temp=filter_form1(*data,num,den,order,xbuffer,ybuffer);
		y=y+temp;
		data++;
	}
	y=y/step;
	return y;
}

double filter_step2(double *data,int step,int measure,double *xbuffer,double *ybuffer,double *num,double *den,int order)
{
	double y=0,temp;
	int i;
	for(i=0;i<measure;i++)
	{
		rotate_buffer(0,ybuffer,measure);
		rotate_buffer(*data,xbuffer,measure); 
		temp=filter_form1_internal(*data,num,den,order,xbuffer,ybuffer);
		data++;
	}
	for(i=0;i<step;i++)
	{
		y=y+*ybuffer;
		ybuffer++;
	}
	
	y=y/step;
	return y;
}


double filter_form1_internal(double x,double *num,double *den, int order,double *xbuffer,double *ybuffer)
{
	double y=0,a,*temp=num,*yout=ybuffer;
	int i;
	a=*num;
	*num=0;

	for(i=0;i<=order;i++,den++,num++,xbuffer++,ybuffer++)
	{
		y=y+(*den)*(*xbuffer)-(*num)*(*ybuffer);
	}
	*temp=a;
	y=y/a;
	*yout=y;
	return y;
}

