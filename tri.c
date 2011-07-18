#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <stdlib.h>
#include <jemalloc/jemalloc.h>
#include "utils.c"

GLuint glprog;

const char *vert_shader_file = "shaders/basic.vert";

const char *frag_shader_file = "shaders/basic.frag";

const float vert_pos[] = {
	0.75f, 0.75f, 0.0f, 1.0f,
	0.75f, -0.75f, 0.0f, 1.0f,
	-0.75f, -0.75f, 0.0f, 1.0f,
};

GLuint posbuf;
GLuint vao;

void render() {
    glClear( GL_COLOR_BUFFER_BIT );

    glUseProgram(glprog);
    glBindBuffer(GL_ARRAY_BUFFER, posbuf);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

    glDrawArrays(GL_TRIANGLES, 0, 3);
    
    glDisableVertexAttribArray(0);
    glUseProgram(0);

    glutSwapBuffers();
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

    glGenBuffers(1, &posbuf);
    glBindBuffer(GL_ARRAY_BUFFER, posbuf);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vert_pos), vert_pos, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode( GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB );

    glutCreateWindow("tritest");
    glutDisplayFunc(render);
    //glutIdleFunc(render);
    glutReshapeFunc(resize);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    glewInit();
	if (!glewIsSupported("GL_VERSION_2_0")) {
		printf("OpenGL 2.0 not supported\n");
		exit(1);
	}

    init();

    glutMainLoop();

    return 0;
}
