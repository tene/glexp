#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <stdlib.h>
#include <jemalloc/jemalloc.h>
#include "utils.c"

GLuint glprog;

const char *vert_shader_file = "shaders/timecirc.vert";

const char *frag_shader_file = "shaders/basic.frag";

const float vert_data[] = {
    0.0f,    0.5f, 0.0f, 1.0f,
    0.5f, -0.366f, 0.0f, 1.0f,
   -0.5f, -0.366f, 0.0f, 1.0f,
    1.0f,    0.0f, 0.0f, 1.0f,
    0.0f,    1.0f, 0.0f, 1.0f,
    0.0f,    0.0f, 1.0f, 1.0f,
};

GLuint posbuf;
GLuint vao;
GLint time_uniform;

void render() {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear( GL_COLOR_BUFFER_BIT );

    glUseProgram(glprog);

    glUniform1f(time_uniform, glutGet(GLUT_ELAPSED_TIME) / 1000.0f);

    glBindBuffer(GL_ARRAY_BUFFER, posbuf);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void *)48);

    glDrawArrays(GL_TRIANGLES, 0, 3);
    
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
    time_uniform = glGetUniformLocation(glprog, "time");

    glGenBuffers(1, &posbuf);
    glBindBuffer(GL_ARRAY_BUFFER, posbuf);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vert_data), vert_data, GL_STREAM_DRAW);
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
