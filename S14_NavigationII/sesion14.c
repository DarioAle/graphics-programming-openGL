/*
 * Practica10.c
 *
 *  Created on: 19/09/2016
 *      Author: Cursos
 */

#include <GL/glew.h>
#include <GL/freeglut.h>
#include "Utils/Utils.h"
#include "Transforms/Transforms.h"
#include <stdio.h>
#include <math.h>

#define DEGREES_2_RADIANS(x) (M_PI * x / 180)

static GLuint programId, va[1], vertexPosLoc, vertexColLoc,
modelMatrixLoc,projMatrixLoc, viewMatrixLoc;
static Mat4 projMatrix;
static GLboolean usePerspective = GL_TRUE;

typedef enum { NONE, FORWARD, BACKWARD, LEFT, RIGHT} Motion;

static Motion motion = NONE;
static float cameraX = 0, cameraZ = 0, cameraAngle = 0;
static float rotationSpeed = 1.2, cameraSpeed = 0.5;
static float angleY = 0, angleZ = 0;

static void speciaKeyPressed(int code, int x, int y)
{
	//	Forward  -> 101
	//  Backward -> 103
	//  Left     -> 100
	//  Right    -> 102

	switch(code){
		case 101: motion = FORWARD; break;
		case 103: motion = BACKWARD; break;
		case 100: motion = LEFT; break;
		case 102: motion = RIGHT; break;
	}
//	printf("Pressed key with code: %d\n", code);

}

static void specialKeyReleased(int code, int x, int y)
{
	motion = NONE;
}

static void initShaders() {
	GLuint vShader = compileShader("shaders/projection.vsh", GL_VERTEX_SHADER, GL_FALSE);
	if (!shaderCompiled(vShader)) return;
	GLuint fShader = compileShader("shaders/color.fsh", GL_FRAGMENT_SHADER, GL_FALSE);
	if (!shaderCompiled(fShader)) return;

	programId = glCreateProgram();
	glAttachShader(programId, vShader);
	glAttachShader(programId, fShader);
	glLinkProgram(programId);
	vertexPosLoc      = glGetAttribLocation(programId, "vertexPosition");
	vertexColLoc      = glGetAttribLocation(programId, "vertexColor");
	modelMatrixLoc    = glGetUniformLocation(programId, "modelMatrix");
	viewMatrixLoc     = glGetUniformLocation(programId, "viewMatrix");
	projMatrixLoc     = glGetUniformLocation(programId, "projMatrix");

	glUseProgram(programId);
	glEnable(GL_DEPTH_TEST);
//	glEnable(GL_CULL_FACE);
//	glFrontFace(GL_CW);
}

// Crea un cuarto de (w x h x d) centrado en el origen
static void createRoom(int w, int h, int d) {
	float x1 = -w/2, x2 = w/2;
	float y1 = -h/2, y2 = h/2;
	float z1 = -d/2, z2 = d/2;
	float roomPos[] = { // Front (far) wall
						 x1, y1, z1,
						 x1, y2, z1,
						 x2, y1, z1,
						 x2, y2, z1,
						 // Back (near) wall
						 x2, y1, z2,
						 x2, y2, z2,
						 x1, y1, z2,
						 x1, y2, z2,
					    // Left wall
						 x1, y1, z2,
						 x1, y2, z2,
						 x1, y1, z1,
						 x1, y2, z1,
						// Right wall
						 x2, y1, z1,
						 x2, y2, z1,
						 x2, y1, z2,
						 x2, y2, z2,
						// Floor
						 x1, y1, z1,
						 x2, y1, z1,
						 x1, y1, z2,
						 x2, y1, z2,
						// Ceiling
						 x2, y2, z1,
						 x1, y2, z1,
						 x2, y2, z2,
						 x1, y2, z2
	};
	float roomCol[] = {     0,  1,  0,
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

							1,  1,  0,
							1,  1,  0,
							1,  1,  0,
							1,  1,  0,

							0,  1,  1,
							0,  1,  1,
							0,  1,  1,
							0,  1,  1
	};
	GLushort roomIndex[] = {  0,  1,  2,  3, 0xFFFF,
				              4,  5,  6,  7, 0xFFFF,
				              8,  9, 10, 11, 0xFFFF,
				             12, 13, 14, 15, 0xFFFF,
							 16, 17, 18, 19, 0xFFFF,
							 20, 21, 22, 23
	};

	glGenVertexArrays(1, va);
	glBindVertexArray(va[0]);
	glBindBuffer(GL_ARRAY_BUFFER, 1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(roomPos), roomPos, GL_STATIC_DRAW);
	glEnableVertexAttribArray(vertexPosLoc);
	glVertexAttribPointer(vertexPosLoc, 3, GL_FLOAT, 0, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(roomCol), roomCol, GL_STATIC_DRAW);
	glEnableVertexAttribArray(vertexColLoc);
	glVertexAttribPointer(vertexColLoc, 3, GL_FLOAT, 0, 0, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 3);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(roomIndex), roomIndex, GL_STATIC_DRAW);

	glEnable(GL_PRIMITIVE_RESTART);
	glPrimitiveRestartIndex(0xFFFF);
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
						  -2, -1,  3,
						   2, -1, -3,
						   2, -1,  3,

						  -2, -1, -3,
						   2, -1, -3,
						  -2,  2, -3,
						   2,  2, -3
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

static void moveForward()
{
	float radians = DEGREES_2_RADIANS(cameraAngle);
	cameraX -= cameraSpeed * sin(radians);
	cameraZ -= cameraSpeed * cos(radians);
}

static void moveBackward()
{
	float radians = DEGREES_2_RADIANS(cameraAngle);
	cameraX += cameraSpeed * sin(radians);
	cameraZ += cameraSpeed * cos(radians);
}

static void rotateLeft()
{
	cameraAngle += rotationSpeed;
}

static void rotateRight()
{
	cameraAngle -= rotationSpeed;
}


static void display() {
	Mat4 modelMat, viewMat;
	mIdentity(&modelMat);
	mIdentity(&viewMat);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(programId);

	switch(motion){
		case FORWARD: moveForward(); break;
		case BACKWARD: moveBackward(); break;
		case LEFT: 		rotateLeft(); break;
		case RIGHT: 	rotateRight(); break;
		case NONE: break;
	}
	rotateY(&viewMat, -cameraAngle);
	translate(&viewMat, -cameraX, 0, -cameraZ);

	glBindVertexArray(va[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 3);

	glUniformMatrix4fv(viewMatrixLoc, 1, GL_TRUE, viewMat.values);
	glUniformMatrix4fv(modelMatrixLoc, 1, GL_TRUE, modelMat.values);
	glDrawElements(GL_TRIANGLE_STRIP, 29, GL_UNSIGNED_SHORT, 0);
	glutSwapBuffers();
}

static void timerFunc(int id) {
	glutTimerFunc(20, timerFunc, id);
	glutPostRedisplay();
}

static void reshapeFunc(int w, int h) {
	glViewport(0, 0, w, h);
	float aspect = (float) w / h;
	if(usePerspective) {
		setPerspective(&projMatrix, 60, aspect, -1, -40);
	} else {
		float cubeLength = 10;
		if(aspect >= 1.0) setOrtho(&projMatrix, -cubeLength * aspect, cubeLength * aspect,
												-cubeLength, cubeLength, -cubeLength, cubeLength);
		else setOrtho(&projMatrix, -cubeLength,  cubeLength, -cubeLength / aspect, cubeLength / aspect,
												-cubeLength, cubeLength);
	}
	glUniformMatrix4fv(projMatrixLoc, 1, GL_TRUE, projMatrix.values);
}

static void exitFunc(unsigned char key, int x, int y) {
    if (key == 27) {
        glDeleteVertexArrays(1, va);
        exit(0);
    }
    if (key == 13) {
    	usePerspective = !usePerspective;
    	int w = glutGet(GLUT_WINDOW_WIDTH);
    	int h = glutGet(GLUT_WINDOW_HEIGHT);
    	reshapeFunc(w, h);
    }
}

int main(int argc, char **argv) {
	setbuf(stdout, NULL);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(100, 100);
    glutTimerFunc(50, timerFunc, 1);

    glutCreateWindow("Mi primera experiencia con coordenadas 3D");
    glutDisplayFunc(display);
    glutKeyboardFunc(exitFunc);
    glutSpecialFunc(speciaKeyPressed);
    glutSpecialUpFunc(specialKeyReleased);
    glutReshapeFunc(reshapeFunc);
    glewInit();
    initShaders();
    createRoom(10, 6, 20);
//    createBox(2, 3, 2);
//    createGlass(12, 6);
    glClearColor(0.05, 0.05, 0.10, 1.0);
    glutMainLoop();
	return 0;
}

