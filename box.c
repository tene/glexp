#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <stdlib.h>
#include <jemalloc/jemalloc.h>
#include "utils.c"
#include "glmath.h"

#define ARRAY_COUNT( array ) (sizeof( array ) / (sizeof( array[0] ) * (sizeof( array ) != sizeof(void*) || sizeof( array[0] ) <= sizeof(void*))))

GLuint glprog;

const char *vert_shader_file = "shaders/perspcamera.vert";

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
    0.25f, 0.25f, 0.25f, // 0 URF
   -0.25f, 0.25f, 0.25f, // 1 ULF
    0.25f,-0.25f, 0.25f, // 2 LRF
   -0.25f,-0.25f, 0.25f, // 3 LLF
    0.25f, 0.25f,-0.25f, // 4 URB
   -0.25f, 0.25f,-0.25f, // 5 ULB
    0.25f,-0.25f,-0.25f, // 6 LRB
   -0.25f,-0.25f,-0.25f, // 7 LLB


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
GLint camera_unif;
GLint perspec_unif;
vec2f bearing = {0, 0};
vec3f position = {-3.0f, -4.0f, -6.5f};
float aspect_ratio = 1;

char world[8][8][8] = {
    { {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}, {1,1,1,1,1,1,1,1}, {1,1,1,1,1,1,1,1}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0} },
    { {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}, {0,1,1,1,1,1,1,0}, {1,1,1,1,1,1,1,1}, {1,1,1,1,1,1,1,1}, {0,1,1,1,1,1,1,0}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0} },
    { {0,0,0,0,0,0,0,0}, {0,0,1,1,1,1,0,0}, {0,1,1,1,1,1,1,0}, {1,1,1,1,1,1,1,1}, {1,1,1,1,1,1,1,1}, {0,1,1,1,1,1,1,0}, {0,0,1,1,1,1,0,0}, {0,0,0,0,0,0,0,0} },
    { {0,0,0,1,1,0,0,0}, {0,0,1,1,1,1,0,0}, {0,1,1,1,1,1,1,0}, {1,1,1,1,1,1,1,1}, {1,1,1,1,1,1,1,1}, {0,1,1,1,1,1,1,0}, {0,0,1,1,1,1,0,0}, {0,0,0,1,1,0,0,0} },
    { {0,0,0,1,1,0,0,0}, {0,0,1,1,1,1,0,0}, {0,1,1,1,1,1,1,0}, {1,1,1,1,1,1,1,1}, {1,1,1,1,1,1,1,1}, {0,1,1,1,1,1,1,0}, {0,0,1,1,1,1,0,0}, {0,0,0,1,1,0,0,0} },
    { {0,0,0,0,0,0,0,0}, {0,0,1,1,1,1,0,0}, {0,1,1,1,1,1,1,0}, {1,1,1,1,1,1,1,1}, {1,1,1,1,1,1,1,1}, {0,1,1,1,1,1,1,0}, {0,0,1,1,1,1,0,0}, {0,0,0,0,0,0,0,0} },
    { {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}, {0,1,1,1,1,1,1,0}, {1,1,1,1,1,1,1,1}, {1,1,1,1,1,1,1,1}, {0,1,1,1,1,1,1,0}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0} },
    { {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}, {1,1,1,1,1,1,1,1}, {1,1,1,1,1,1,1,1}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0} },
};

void render() {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0f);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glUseProgram(glprog);

    glBindVertexArray(vao);

    for (int x=0; x<8; x++) {
        for (int y=0; y<8; y++) {
            for (int z=0; z<8; z++) {
                if (world[x][y][z] == 1) {
                    glUniform3f(offset_unif, x/2.0f, y/2.0f, z/2.0f);
                    glDrawElements(GL_TRIANGLES, ARRAY_COUNT(index_data), GL_UNSIGNED_SHORT, 0);
                }
            }
        }
    }

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
    perspective_matrix(75.0f, 0.5f, 100.0f, aspect_ratio, perspec);

    glUseProgram(glprog);
    glUniformMatrix4fv(perspec_unif, 1, GL_TRUE, (GLfloat *)perspec);
    glUseProgram(0);
}

void setup_camera() {
    mat44f camera;
    mat44f pos;
    mat44f rot;
    mat44f tmp;
    //mat44f total;
    translation_matrix(0, 1, 0, camera);
    translation_matrix(position[0], position[1], position[2], pos);
    rotation_matrix(bearing[0], bearing[1], rot);
    mmmul4f(rot, pos, tmp);
    //mmmul4f(tmp, camera, total);

    glUseProgram(glprog);
    glUniformMatrix4fv(camera_unif, 1, GL_TRUE, (GLfloat *)tmp);
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
    camera_unif = glGetUniformLocation(glprog, "camera");
    perspec_unif = glGetUniformLocation(glprog, "perspective");
    setup_perspective();
    setup_camera();

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

void keyboard(unsigned char key, int x, int y)
{
    switch (key) {
        case 27:
            //cleanup();
            glutLeaveMainLoop();
            break;
        case 'w': position[2] += 1; break;
        case 's': position[2] -= 1; break;
        case 'a': position[0] += 1; break;
        case 'd': position[0] -= 1; break;
        case 'r': position[1] -= 1; break;
        case 'f': position[1] += 1; break;

        case 'q': bearing[0] += 0.1; break;
        case 'e': bearing[0] -= 0.1; break;
        case 'g': bearing[1] -= 0.1; break;
        case 't': bearing[1] += 0.1; break;
    }
    setup_camera();
    glutPostRedisplay();
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode( GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB );

    glutCreateWindow("boxtest");
    glutDisplayFunc(render);
    //glutIdleFunc(render);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyboard);

    glewInit();
    if (!glewIsSupported("GL_VERSION_2_0")) {
        printf("OpenGL 2.0 not supported\n");
        exit(1);
    }

    init();

    glutMainLoop();

    return 0;
}
