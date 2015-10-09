#include <math.h>


void subtracVec(float* a1, float* a2, float* result)
{
    result[0] = a1[0] - a2[0];
    result[1] = a1[1] - a2[1];
    result[2] = a1[2] - a2[2];
}    

void prodsum(const float* scalar, float* v, float* a1, float* result)
{
    result[0] = (*scalar) * v[0] + a1[0];
    result[1] = (*scalar) * v[1] + a1[1];
    result[2] = (*scalar) * v[2] + a1[2];
}

void prodsumsum(const float* scalar, float* v, float* a1, float* a2, float* result)
{
    result[0] = (*scalar) * v[0] + a1[0] + a2[0];
    result[1] = (*scalar) * v[1] + a1[1] + a2[1];
    result[2] = (*scalar) * v[2] + a1[2] + a2[2];
}    


