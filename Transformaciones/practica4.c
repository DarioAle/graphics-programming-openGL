///*
// * practica4.c
// *
// *  Created on: 27/02/2020
// *      Author: Dario
// */
//
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "Utils/Utils.h"
#include "Transforms/Transforms.h"
#include <math.h>
#include <stdio.h>

#define SIDES          30
#define LAYERS         5
#define LAYER_WIDTH    0.12
#define RADIO          1.0
#define START_ANGLE    45
#define TOTAL_DEGREES 270.0

static float polygonPos[((SIDES + 1) * (LAYERS * 2)) * 2] = {};
static float polygonColor[(SIDES + 1) * (LAYERS * 2) * 3] = {};

static GLuint va[1];
static GLuint bufferId[3];
static GLuint programId;
static GLuint vertexPosLoc, vertexColLoc;

GLushort hexagonIndex[ ((SIDES + 1) * (LAYERS * 2)) + LAYERS ] = { };

float to_radians(float);
void createParametricPoints();

// ------------ Events -------------------
void timerFunc(int time)
{
	glutTimerFunc(20, timerFunc, 1);
	glutPostRedisplay();
}

void exitFunc(unsigned char key, int mouseX , int mouseY)
{
	if (key == 27)
	{
//	   glDeleteVertexArrays(1, va);
	   exit(0);
	}
}

static void initShaders() {
	GLuint vShader = compileShader("shaders/vertexPosition_practica4.vsh", GL_VERTEX_SHADER);
	GLuint fShader = compileShader("shaders/color.fsh", GL_FRAGMENT_SHADER);
	if (!shaderCompiled(vShader)) return;
	if (!shaderCompiled(fShader)) return;

	programId = glCreateProgram();
	glAttachShader(programId, vShader);
	glAttachShader(programId, fShader);
	glLinkProgram(programId);
	vertexPosLoc   = glGetAttribLocation(programId, "vertexPosition");
	vertexColLoc   = glGetAttribLocation(programId, "vertexColor");


}

void display()
{
	glPointSize(5.0);

	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(programId);
	glBindVertexArray(va[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferId[2]);
	glDrawElements(GL_TRIANGLE_STRIP, ((SIDES + 1) * (LAYERS * 2)) + LAYERS - 1, GL_UNSIGNED_SHORT, 0);
//	glDrawArrays(GL_TRIANGLE_STRIP, 0, (SIDES + 1) * (LAYERS + 1));

	glutSwapBuffers();
}

void initVertex()
{
	glGenVertexArrays(1, va);
	glBindVertexArray(va[0]);
	glGenBuffers(3, bufferId);

	glBindBuffer(GL_ARRAY_BUFFER, bufferId[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(polygonPos), polygonPos, GL_STATIC_DRAW);
	glVertexAttribPointer(vertexPosLoc, 2, GL_FLOAT, 0, 0, 0);
	glEnableVertexAttribArray(vertexPosLoc);

	glBindBuffer(GL_ARRAY_BUFFER, bufferId[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(polygonColor), polygonColor, GL_STATIC_DRAW);
	glVertexAttribPointer(vertexColLoc, 3, GL_FLOAT, 0, 0, 0);
	glEnableVertexAttribArray(vertexColLoc);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferId[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(hexagonIndex), hexagonIndex, GL_STATIC_DRAW);

	glEnable(GL_PRIMITIVE_RESTART);
	glPrimitiveRestartIndex(0xFFFF);
}

int main(int argc, char** argv)
{
	setbuf(stdout, NULL);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(100, 100);

    glutCreateWindow("Practica 4");
//    glutEnterGameMode();

    glutTimerFunc(50, timerFunc, 1);
    glutDisplayFunc(display);
    glutKeyboardFunc(exitFunc);
    glewInit();

    createParametricPoints();
    initShaders();
    initVertex();

    glClearColor(0.7098, 0.8274, 0.9019, 1.0);
    glutMainLoop();
	return 0;
}

float to_radians(float degrees)
{
	return degrees * (M_PI / 180.0 );
}

void createParametricPoints()
{
	int index = 0;
	int colorIndex = 0;
	int bufferIndex = 0;

	float radio = RADIO;
	float const increment = TOTAL_DEGREES / SIDES;

	float r = 0, g = 0, b = 0;

	for(int l = 0; l < LAYERS; l++)
	{
		r = rand()/(RAND_MAX*1.0);
		g = rand()/(RAND_MAX*1.0);

		// Generating Indexs by layer
		for(int a = 0; a < (SIDES + 1) * 2; a++, bufferIndex++)
		{
			hexagonIndex[bufferIndex + l] = bufferIndex;
		}
		hexagonIndex[bufferIndex + l] = 0xFFFF;

		// Generating points by layer
		float j = START_ANGLE;
		for(int i = 0 ; i < SIDES + 1; i++, j += increment, index++)
		{
			polygonPos[index * 4] = radio * cos(to_radians(j));
			polygonPos[(index * 4) + 1] = radio * sin(to_radians(j));

			polygonPos[(index * 4) + 2] = (radio - LAYER_WIDTH) * cos(to_radians(j));
			polygonPos[(index * 4) + 3] = (radio - LAYER_WIDTH) * sin(to_radians(j));

		}
		// Assigning colors by layer
		for(int k = 0; k < ((SIDES + 1) * 2); k++, colorIndex++)
		{
			b = rand()/(RAND_MAX*1.0);
			polygonColor[colorIndex * 3] = r;
			polygonColor[colorIndex * 3 + 1] = g;
			polygonColor[colorIndex * 3 + 2] = b;
		}
		radio -= LAYER_WIDTH;
	}



}
