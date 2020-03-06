/*
 * examen1_4.c
 *
 *  Created on: 05/03/2020
 *      Author: Dario
 */
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "Utils/Utils.h"
#include "Transforms/Transforms.h"
#include <stdio.h>

#define ROWS    20
#define COLUMNS 20

#define ROW_WIDTH (1.96 / ROWS)
#define COLUMN_WIDTH (1.96 / COLUMNS)

#define START
GLushort meshIndex[ ((2 * (COLUMNS + 1)) * ROWS) + ROWS - 1] = {};
static float meshPos[((2 * (COLUMNS + 1)) * ROWS) * 2] = {};


//static float meshColor[((2 * (COLUMNS + 1)) + (ROWS)) * 3] = {};

static GLuint vertexColLoc, vertexPositionLoc,programId;
static GLuint va[1] = {};
static GLuint bufferId[3] = {};

static void display();
static void initShaders();



static void pressEnter(unsigned char key, int x, int y)
{

	display();
}

static void init() {


	glGenVertexArrays(1, va);
	glBindVertexArray(va[0]);
	glGenBuffers(3, bufferId);


	glBindBuffer(GL_ARRAY_BUFFER, bufferId[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(meshPos), meshPos, GL_STATIC_DRAW);
	glVertexAttribPointer(vertexPositionLoc, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vertexPositionLoc);

//	glBindBuffer(GL_ARRAY_BUFFER, bufferId2);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);
//	glVertexAttribPointer(vertexColLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
//	glEnableVertexAttribArray(vertexColLoc);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferId[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(meshIndex), meshIndex, GL_STATIC_DRAW);

	glEnable(GL_PRIMITIVE_RESTART);
	glPrimitiveRestartIndex(0xFFFF);

}

static void createMesh()
{
//	printf("size of mesh: %d", sizeof(meshPos));

	float startPointy = 0.98 - ROW_WIDTH;
	int columnIndex = 0;
	int bufferIndex = 0;

	for(int i = 0; i < ROWS; i++)
	{
		float startPointx = -0.98;
		for(int j = 0; j < COLUMNS + 1; j++)
		{
			meshPos[(columnIndex * 4)] = startPointx;
			meshPos[(columnIndex * 4) + 1] = startPointy;

			meshPos[(columnIndex * 4) + 2] = startPointx;
			meshPos[(columnIndex * 4) + 3] = startPointy + ROW_WIDTH;

//			printf("(%f, %f) , (%f, %f)\n", meshPos[j *4], meshPos[(j *4) + 1], meshPos[(j *4) + 2], meshPos[(j *4) + 3]);

			startPointx += COLUMN_WIDTH;
			columnIndex++;
		}

		for(int a = 0; a < (COLUMNS + 1) * 2; a++, bufferIndex++)
		{
			meshIndex[bufferIndex + i] = bufferIndex;
		}
		meshIndex[bufferIndex + i] = 0xFFFF;

		printf("\n");
		startPointy -= ROW_WIDTH;

 	}

	for(int i = 0; i < 37; i++)
	{
//		printf("%d\n", meshIndex[i]);
	}
}



static void display() {
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(programId);

	glPointSize(5.0);

	glBindVertexArray(va[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferId[2]);
	glDrawElements(GL_LINE_STRIP,(((2 * (COLUMNS + 1)) * ROWS) + ROWS - 1), GL_UNSIGNED_SHORT, 0);


	glutSwapBuffers();
}


int main(int argc, char** argv) {
	setbuf(stdout, NULL);

	glutInit(&argc, argv);
	int w = 600, h = 600;
	int x = (glutGet(GLUT_SCREEN_WIDTH) - w) / 2;
	int y = (glutGet(GLUT_SCREEN_HEIGHT) - h) / 2;


	glutInitWindowSize(w, h);
	glutInitWindowPosition(x, y);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);

	glutCreateWindow("Exam 1.1: Julia´s Set");
	glutDisplayFunc(display);
	glewInit();
	glutKeyboardFunc(pressEnter);
	glClearColor(0.95, 0.95, 1.0, 1.0);

	initShaders();
	createMesh();
	init();

	glutMainLoop();
	return 0;
}

static void initShaders() {
	GLuint vShaderId = compileShader("shaders/vertexPosition.vsh", GL_VERTEX_SHADER, GL_FALSE);
	if(!shaderCompiled(vShaderId)) return;

	GLuint fShaderId = compileShader("shaders/passTroughShader.fsh", GL_FRAGMENT_SHADER, GL_FALSE);
	if(!shaderCompiled(fShaderId)) return;

	programId = glCreateProgram();
	glAttachShader(programId, vShaderId);
	glAttachShader(programId, fShaderId);
	glLinkProgram(programId);

	vertexPositionLoc = glGetAttribLocation (programId, "vertexPosition");	// Vertex Shader
	vertexColLoc      = glGetAttribLocation (programId, "vertexColor");		// Vertex shader


}
