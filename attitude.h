void QuatMult(float* qa,float* qb,float* qOut,int conj);
void Quat2Rot(float* q,float** M);
void UpdateQuat(float *q, float *mu, float *qf, float control_period);
void Quat2Euler(float* q, float* angles, unsigned char sequence);
void Rot2Quat(const float** M,float* q);
void UpdateQuatFAST(float *quat,float *ome, float *qp);
void Euler2Rot(const float* eul, float **m, const unsigned int b2f); //sequenza 1-2-3
void Euler2Quat(const float *euler, float *qout, const unsigned int b2f );

