#include <stdio.h>

#define			PI	3.141592

extern void		InvertMat3(float** x,float** invx);

extern void		MultVByScl(float* x,float a,float* y,int N);
extern void		MultMByScl(float** x,float a,float** y,int Nr,int Nc);
extern void		MultVByScl3(float* x,float a,float* y);
extern void		MultMByScl3(float** x,float a,float** y);

extern float	SumVEl(float* x,int N);
extern float	SumMColEl(float** x,int N,int k);

extern void		DotPMV(float** m,float* x,float* dotP, int Nr, int Nc);
extern void		DotPMV3(float** m,float* x,float* dotP);
void DotPMM(float** x1, float** x2, float** dotP, int Nr1, int Nc1, int Nr2, int Nc2);
extern void		DotPMM3(float** x1,float** x2,float** dotP);

extern void		AddV(float* x1,float* x2,float* y,int N);
extern void		AddV3(float* x1,float* x2,float* y);
extern void		AddM(float** x1,float** x2,float** y,int Nr,int Nc);
extern void		AddM3(float** x1,float** x2,float** y);

extern void		Mat2Vec(float** m,float* y,int Nr,int Nc);
extern void		Mat2Vec3(float** m,float* y);

extern void		Vec2Mat(float* x,float** m,int N);
extern void		Vec2Mat3(float* x,float** m);

extern void		SaveMatIntoDatFile(float** m,int Nr,int Nc,char* fileName);

extern float	Sat(float x,float min, float max);
extern float	Abs(float x);
extern float	Max(float *x, int N);
extern float	Min(float *x, int N);
extern float	Mean(float *x, int N);

extern float	NormV3(float *x);
extern float	NormV4(float *x);
extern void		NormalizeV4(float *q);

extern float*	polcar(float **A,long int n);







