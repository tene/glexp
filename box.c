#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <stdlib.h>
#include <jemalloc/jemalloc.h>
#include "utils.c"
#include "glmath.h"

GLuint glprog;

const char *vert_shader_file = "shaders/persp.vert";

const char *frag_shader_file = "shaders/basic.frag";

const float vert_data[] = {
    0.25f,  0.25f, -1.25f, 1.0f,
	 0.25f, -0.25f, -1.25f, 1.0f,
	-0.25f,  0.25f, -1.25f, 1.0f,

	 0.25f, -0.25f, -1.25f, 1.0f,
	-0.25f, -0.25f, -1.25f, 1.0f,
	-0.25f,  0.25f, -1.25f, 1.0f,

	 0.25f,  0.25f, -2.75f, 1.0f,
	-0.25f,  0.25f, -2.75f, 1.0f,
	 0.25f, -0.25f, -2.75f, 1.0f,

	 0.25f, -0.25f, -2.75f, 1.0f,
	-0.25f,  0.25f, -2.75f, 1.0f,
	-0.25f, -0.25f, -2.75f, 1.0f,

	-0.25f,  0.25f, -1.25f, 1.0f,
	-0.25f, -0.25f, -1.25f, 1.0f,
	-0.25f, -0.25f, -2.75f, 1.0f,

	-0.25f,  0.25f, -1.25f, 1.0f,
	-0.25f, -0.25f, -2.75f, 1.0f,
	-0.25f,  0.25f, -2.75f, 1.0f,

	 0.25f,  0.25f, -1.25f, 1.0f,
	 0.25f, -0.25f, -2.75f, 1.0f,
	 0.25f, -0.25f, -1.25f, 1.0f,

	 0.25f,  0.25f, -1.25f, 1.0f,
	 0.25f,  0.25f, -2.75f, 1.0f,
	 0.25f, -0.25f, -2.75f, 1.0f,

	 0.25f,  0.25f, -2.75f, 1.0f,
	 0.25f,  0.25f, -1.25f, 1.0f,
	-0.25f,  0.25f, -1.25f, 1.0f,

	 0.25f,  0.25f, -2.75f, 1.0f,
	-0.25f,  0.25f, -1.25f, 1.0f,
	-0.25f,  0.25f, -2.75f, 1.0f,

	 0.25f, -0.25f, -2.75f, 1.0f,
	-0.25f, -0.25f, -1.25f, 1.0f,
	 0.25f, -0.25f, -1.25f, 1.0f,

	 0.25f, -0.25f, -2.75f, 1.0f,
	-0.25f, -0.25f, -2.75f, 1.0f,
	-0.25f, -0.25f, -1.25f, 1.0f,




	0.0f, 0.0f, 1.0f, 1.0f,
	0.0f, 0.0f, 1.0f, 1.0f,
	0.0f, 0.0f, 1.0f, 1.0f,

	0.0f, 0.0f, 1.0f, 1.0f,
	0.0f, 0.0f, 1.0f, 1.0f,
	0.0f, 0.0f, 1.0f, 1.0f,

	0.8f, 0.8f, 0.8f, 1.0f,
	0.8f, 0.8f, 0.8f, 1.0f,
	0.8f, 0.8f, 0.8f, 1.0f,

	0.8f, 0.8f, 0.8f, 1.0f,
	0.8f, 0.8f, 0.8f, 1.0f,
	0.8f, 0.8f, 0.8f, 1.0f,

	0.0f, 1.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 0.0f, 1.0f,

	0.0f, 1.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 0.0f, 1.0f,

	0.5f, 0.5f, 0.0f, 1.0f,
	0.5f, 0.5f, 0.0f, 1.0f,
	0.5f, 0.5f, 0.0f, 1.0f,

	0.5f, 0.5f, 0.0f, 1.0f,
	0.5f, 0.5f, 0.0f, 1.0f,
	0.5f, 0.5f, 0.0f, 1.0f,

	1.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f, 1.0f,

	1.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f, 1.0f,

	0.0f, 1.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 1.0f, 1.0f,

	0.0f, 1.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 1.0f, 1.0f,
};

GLuint posbuf;
GLuint vao;
GLint offset_unif;
GLint perspec_unif;

void render() {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear( GL_COLOR_BUFFER_BIT );

    glUseProgram(glprog);

    glUniform2f(offset_unif, 0.5f, 0.5f);

    size_t color_index = sizeof(vert_data) / 2;
    glBindBuffer(GL_ARRAY_BUFFER, posbuf);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void *)color_index);

    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glUseProgram(0);

    glutSwapBuffers();
    glutPostRedisplay();
}

void resize(int w, int h) {
    h = h ? h : 1;
    glViewport(0, 0, w, h);
}

void init() {
    GLuint shaders[2];
    shaders[0] = load_shader(GL_VERTEX_SHADER, vert_shader_file);
    shaders[1] = load_shader(GL_FRAGMENT_SHADER, frag_shader_file);

    glprog = create_program(2, shaders);
    offset_unif = glGetUniformLocation(glprog, "offset");
    perspec_unif = glGetUniformLocation(glprog, "perspective");
    mat44f perspec;
    perspective_matrix(1.0f, 0.5f, 3.0f, perspec);
    for (int i = 0; i<16; i++) {
        printf("%2f\n", ((float *)perspec)[i]);
    }

    glUseProgram(glprog);
    glUniformMatrix4fv(perspec_unif, 1, GL_TRUE, (GLfloat *)perspec);
    glUseProgram(0);

    glGenBuffers(1, &posbuf);
    glBindBuffer(GL_ARRAY_BUFFER, posbuf);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vert_data), vert_data, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode( GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB );

    glutCreateWindow("tritest");
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
