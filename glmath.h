#include <string.h>
#include <GL/glew.h>
#include <cblas.h>
typedef GLfloat vec4f[4];
typedef GLfloat mat44f[4][4];

#define mvmul4f(m,v,r) cblas_dgemv(CblasRowMajor, CblasNoTrans, 4, 4, 1.0, (float *)m, 4, (float *)v, 1, 0.0, r, 1);

void perspective_matrix(float scale, float znear, float zfar, mat44f p) {
    memset(p, 0, 16 * sizeof(float));
    p[0][0] = scale;
    p[1][1] = scale;
    p[2][2] = (znear+zfar)/(znear-zfar);
    p[3][2] = (2*znear*zfar)/(znear-zfar);
    p[2][3] = -1.0f;
}
