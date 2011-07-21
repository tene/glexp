#include <cblas.h>
typedef double vec4d[4];
typedef double mat44d[4][4];

#define mvmul4d(m,v,r) cblas_dgemv(CblasRowMajor, CblasNoTrans, 4, 4, 1.0, (double *)m, 4, (double *)v, 1, 0.0, r, 1);
