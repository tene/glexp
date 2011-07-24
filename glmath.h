#include <string.h>
#include <GL/glew.h>
#include <cblas.h>
#include <math.h>
typedef GLfloat vec2f[2];
typedef GLfloat vec3f[3];
typedef GLfloat vec4f[4];
typedef GLfloat mat44f[4][4];

#define mvmul4f(m,v,r) cblas_sgemv(CblasRowMajor, CblasNoTrans, 4, 4, 1.0, (float *)m, 4, (float *)v, 1, 0.0, (float *)r, 1);
#define mmmul4f(a,b,r) cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, 4, 4, 4, 1.0, (float *)a, 4, (float *)b, 4, 0.0, (float *)r, 4);

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

void rotation_matrix(float azimuth, float inclination, mat44f r) {
    mat44f yaw;
    mat44f pitch;
    memset(r, 0, 16*sizeof(float));
    memset(yaw, 0, 16*sizeof(float));
    memset(pitch, 0, 16*sizeof(float));
    pitch[1][1] = cos(inclination);
    pitch[1][2] = sin(inclination);
    pitch[2][1] = -1 * pitch[1][2];
    pitch[2][2] = pitch[1][1];
    pitch[0][0] = 1;
    pitch[3][3] = 1;

    yaw[0][0] = cos(azimuth);
    yaw[2][0] = sin(azimuth);
    yaw[0][2] = -1 * yaw[2][0];
    yaw[2][2] = yaw[0][0];
    yaw[1][1] = 1;
    yaw[3][3] = 1;

    mmmul4f(pitch, yaw, r);
}
