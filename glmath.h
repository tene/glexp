#include <string.h>
#include <GL/glew.h>
#include <cblas.h>
#include <math.h>
typedef GLfloat vec4f[4];
typedef GLfloat mat44f[4][4];

#define mvmul4f(m,v,r) cblas_dgemv(CblasRowMajor, CblasNoTrans, 4, 4, 1.0, (float *)m, 4, (float *)v, 1, 0.0, r, 1);

void perspective_matrix(float fovdeg, float znear, float zfar, float aspect_ratio, mat44f p) {
    float scale =  1.0f / tan(fovdeg * 3.14159f / 360.0f);
    memset(p, 0, 16 * sizeof(float));
    p[0][0] = scale * aspect_ratio;
    p[1][1] = scale;
    p[2][2] = (znear+zfar)/(znear-zfar);
    p[3][2] = (2*znear*zfar)/(znear-zfar);
    p[2][3] = -1.0f;
}

void translation_matrix(float x, float y, float z, mat44f p) {
    memset(p, 0, 16*sizeof(float));
    for (int i =0; i<4; i++) {
        p[i][i] = 1;
    }
    p[0][3] = x;
    p[1][3] = y;
    p[2][3] = z;
}
