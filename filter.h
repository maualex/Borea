#if !defined(FILTER_H)
#define FILTER_H

double filter_form1(double x,double *num,double *den, int order,double *xbuffer,double *ybuffer);
double filter_form1_FAST(double x,double *num,double *den, int order,double *xbuffer,double *ybuffer);

void rotate_buffer(double y,double *buffer,int size);
double filter_step(double *data,int step,double *xbuffer,double *ybuffer,double *num,double *den,int order);
double filter_step2(double *data,int step,int measure,double *xbuffer,double *ybuffer,double *num,double *den,int order);
double filter_form1_internal(double x,double *num,double *den, int order,double *xbuffer,double *ybuffer);
#  endif  
