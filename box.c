#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <stdlib.h>
#include <jemalloc/jemalloc.h>
#include "utils.c"
#include "glmath.h"

#define ARRAY_COUNT( array ) (sizeof( array ) / (sizeof( array[0] ) * (sizeof( array ) != sizeof(void*) || sizeof( array[0] ) <= sizeof(void*))))

GLuint glprog;

const char *vert_shader_file = "shaders/persp.vert";

const char *frag_shader_file = "shaders/basic.frag";

const GLshort index_data[] = {
    0, 2, 1,
    2, 3, 1,

    1, 3, 7,
    1, 7, 5,

    3, 2, 6,
    3, 6, 7,

    7, 4, 5,
    7, 6, 4,

    1, 4, 0,
    1, 5, 4,

    0, 4, 2,
    4, 6, 2,
};

int vert_count= 8;
const GLfloat vert_data[] = {
    0.25f, 0.25f,-1.25f, // 0 URF
   -0.25f, 0.25f,-1.25f, // 1 ULF
    0.25f,-0.25f,-1.25f, // 2 LRF
   -0.25f,-0.25f,-1.25f, // 3 LLF
    0.25f, 0.25f,-2.75f, // 4 URB
   -0.25f, 0.25f,-2.75f, // 5 ULB
    0.25f,-0.25f,-2.75f, // 6 LRB
   -0.25f,-0.25f,-2.75f, // 7 LLB


	0.2f, 0.2f, 0.2f, 1.0f,
	1.0f, 0.2f, 0.2f, 1.0f,
	0.2f, 1.0f, 0.2f, 1.0f,
	1.0f, 1.0f, 0.2f, 1.0f,
	0.2f, 0.2f, 1.0f, 1.0f,
	1.0f, 0.2f, 1.0f, 1.0f,
	0.2f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
};

GLuint posbuf;
GLuint idxbuf;
GLuint vao;
GLint offset_unif;
GLint perspec_unif;
float aspect_ratio = 1;

void render() {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0f);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glUseProgram(glprog);

    glBindVertexArray(vao);

    glUniform3f(offset_unif, 1.5f, 0.5f, 0.0f);
    glDrawElements(GL_TRIANGLES, ARRAY_COUNT(index_data), GL_UNSIGNED_SHORT, 0);

    glUniform3f(offset_unif, -1.5f, 0.5f, 0.0f);
    glDrawElements(GL_TRIANGLES, ARRAY_COUNT(index_data), GL_UNSIGNED_SHORT, 0);

    glUniform3f(offset_unif, -1.2f, 0.2f, -0.3f);
    glDrawElements(GL_TRIANGLES, ARRAY_COUNT(index_data), GL_UNSIGNED_SHORT, 0);

    glBindVertexArray(0);
    glUseProgram(0);

    glutSwapBuffers();
    //glutPostRedisplay();
}

void setup_perspective() {
    mat44f perspec;
    perspective_matrix(1.0f, 0.5f, 3.0f, aspect_ratio, perspec);

    glUseProgram(glprog);
    glUniformMatrix4fv(perspec_unif, 1, GL_TRUE, (GLfloat *)perspec);
    glUseProgram(0);
}

void resize(int w, int h) {
    h = h ? h : 1;
    aspect_ratio = (float)h / (float)w;
    setup_perspective();
    glViewport(0, 0, w, h);
    //glutPostRedisplay();
}

void init() {
    GLuint shaders[2];
    shaders[0] = load_shader(GL_VERTEX_SHADER, vert_shader_file);
    shaders[1] = load_shader(GL_FRAGMENT_SHADER, frag_shader_file);

    glprog = create_program(2, shaders);
    offset_unif = glGetUniformLocation(glprog, "offset");
    perspec_unif = glGetUniformLocation(glprog, "perspective");
    setup_perspective();

    glGenBuffers(1, &posbuf);
    glBindBuffer(GL_ARRAY_BUFFER, posbuf);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vert_data), vert_data, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &idxbuf);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idxbuf);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index_data), index_data, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    size_t color_index = sizeof(float) * 3 * vert_count;
    glBindBuffer(GL_ARRAY_BUFFER, posbuf);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void *)color_index);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idxbuf);

    glBindVertexArray(0);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);
    glDepthRange(0.0f, 1.0f);
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode( GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB );

    glutCreateWindow("boxtest");
    glutDisplayFunc(render);
    //glutIdleFunc(render);
    glutReshapeFunc(resize);

    glewInit();
    if (!glewIsSupported("GL_VERSION_2_0")) {
        printf("OpenGL 2.0 not supported\n");
        exit(1);
    }

    init();

    glutMainLoop();

    return 0;
}
