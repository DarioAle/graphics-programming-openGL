/*
 * sesion11.c
 *
 *  Created on: 27/02/2020
 *      Author: Dario
 */

#include <GL/glew.h>
#include <GL/freeglut.h>
#include "Utils/Utils.h"
#include "Transforms/Transforms.h"

static GLuint programId, va[1], vertexPosLoc, vertexColLoc, csMatrixLoc;
static Mat4 projMatrix;
static GLuint projMatrixLoc;

static void initShaders() {
	GLuint vShader = compileShader("shaders/projection.vsh", GL_VERTEX_SHADER);
	if (!shaderCompiled(vShader)) return;
	GLuint fShader = compileShader("shaders/color.fsh", GL_FRAGMENT_SHADER);
	if (!shaderCompiled(fShader)) return;

	programId = glCreateProgram();
	glAttachShader(programId, vShader);
	glAttachShader(programId, fShader);
	glLinkProgram(programId);
	vertexPosLoc   = glGetAttribLocation (programId, "vertexPosition");
	vertexColLoc   = glGetAttribLocation (programId, "vertexColor");
	csMatrixLoc    = glGetUniformLocation(programId, "csMatrix");
	projMatrixLoc  = glGetUniformLocation(programId, "projMatrix");

	glUseProgram(programId);
	setOrtho(&projMatrix, -6, 10, -6, 10, -6, 10);
	glUniformMatrix4fv(projMatrixLoc,1, GL_TRUE, projMatrix.values);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
}

static void createSquare() {
	float squarePos[] = { -2, -1,  3,
			              -2,  1,  3,
			               2, -1,  3,
			               2,  1,  3,

			               2, -1,  3,
			               2,  1,  3,
			               2, -1, -3,
			               2,  2, -3,

			               2,  1,  3,
			              -2,  1,  3,
			               2,  2, -3,
			              -2,  2, -3,

			              -2, -1, -3,
			              -2,  2, -3,
			              -2, -1,  3,
			              -2,  1,  3,

						  -2, -1, -3,
						   2, -1, -3,
						  -2,  2, -3,
						   2,  2, -3,

						   2, -1,  3,
						   2, -1, -3,
						  -2, -1,  3,
						  -2, -1, -3

	};

	float squareCol[] = {  0,  1,  0,
						   0,  1,  0,
					       0,  1,  0,
						   0,  1,  0,

						   1,  0,  0,
						   1,  0,  0,
						   1,  0,  0,
						   1,  0,  0,

						   0,  0,  1,
						   0,  0,  1,
						   0,  0,  1,
						   0,  0,  1,

						   1,  0,  1,
						   1,  0,  1,
						   1,  0,  1,
						   1,  0,  1,

						   0,  1,  1,
						   0,  1,  1,
						   0,  1,  1,
						   0,  1,  1,

						   1,  1,  0,
						   1,  1,  0,
						   1,  1,  0,
						   1,  1,  0

	};

	GLushort squareIndex[] = {  0,  1,  2,  3, 0xFFFF,
			                    4,  5,  6,  7, 0xFFFF,
			                    8,  9, 10, 11, 0xFFFF,
			                   12, 13, 14, 15, 0xFFFF,
							   16, 17, 18, 19, 0xFFFF,
							   20, 21, 22, 23
	};

	glGenVertexArrays(1, va);
	glBindVertexArray(va[0]);
	glBindBuffer(GL_ARRAY_BUFFER, 1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(squarePos), squarePos, GL_STATIC_DRAW);
	glEnableVertexAttribArray(vertexPosLoc);
	glVertexAttribPointer(vertexPosLoc, 3, GL_FLOAT, 0, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(squareCol), squareCol, GL_STATIC_DRAW);
	glEnableVertexAttribArray(vertexColLoc);
	glVertexAttribPointer(vertexColLoc, 3, GL_FLOAT, 0, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 3);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(squareIndex), squareIndex, GL_STATIC_DRAW);

	glEnable(GL_PRIMITIVE_RESTART);
	glPrimitiveRestartIndex(0xFFFF);
}

static float pitch, yaw, roll = 0;

static void display() {
	Mat4 csMat;
	mIdentity(&csMat);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(programId);
	glBindVertexArray(va[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 3);
	pitch += 3;
	yaw   += 1;
	roll  += 9;
	rotateY(&csMat, pitch);
	rotateX(&csMat, yaw);
	rotateZ(&csMat, roll);
	glUniformMatrix4fv(csMatrixLoc, 1, GL_TRUE, csMat.values);
	glDrawElements(GL_TRIANGLE_STRIP, 29, GL_UNSIGNED_SHORT, 0);
	glutSwapBuffers();
}

static void exitFunc(unsigned char key, int x, int y) {
    if (key == 27) {
        glDeleteVertexArrays(1, va);
        exit(0);
    }
}

static void timerFunc(int id) {
	glutTimerFunc(20, timerFunc, id);
	glutPostRedisplay();
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(100, 100);
    glutTimerFunc(50, timerFunc, 1);

    glutCreateWindow("Mi primera experiencia con coordenadas 3D");
    glutDisplayFunc(display);
    glutKeyboardFunc(exitFunc);
    glewInit();
    initShaders();
    createSquare();
    glClearColor(0.05, 0.05, 0.10, 1.0);
    glutMainLoop();
	return 0;
}


