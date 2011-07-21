#include <stdio.h>
#include "glmath.h"

int main() {
    vec4d v = {1,2,3,4};
    mat44d m = {{0,1,0,0},{1,0,0,0},{0,0,1,0},{0,0,0,1}};
    vec4d r = {0, 0, 0};
    mvmul4d(m, v, r);
    printf("%f, %f, %f, %f\n", r[0], r[1], r[2], r[3]);
}
