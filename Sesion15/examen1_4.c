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
#include <time.h>
#include <math.h>

// Input parameters
#define ROWS    10
#define COLUMNS 10

#define ROW_WIDTH    1.0       // Whole normalized square is two units wide
#define COLUMN_WIDTH 1.0 // Whole normalized square is two units high

// Initialize three buffers
GLushort meshIndex    [((2 * (COLUMNS + 1)) * ROWS) + ROWS - 1] = {};
static float meshPos  [((2 * (COLUMNS + 1)) * ROWS) * 3]        = {};
static float meshColor[((2 * (COLUMNS + 1)) * ROWS) * 3]        = {};

// buffer id and attributes location
static GLuint vertexColLoc, vertexPositionLoc,programId;
static GLuint va[1] = {};
static GLuint bufferId[3] = {};
static GLboolean usePerspective = GL_TRUE;
static Mat4 projMatrix;
static GLuint modelMatrixLoc,projMatrixLoc, viewMatrixLoc;


// Function definitions
static void display();
static void initShaders();
static void createMesh();
static void init();

GLuint primitive = GL_TRIANGLE_STRIP;

// When you press enter it produces a different random terrain with the 3rd dimension being the
// color assigned to the area by the rasterizer

static void printVertexPositions()
{
	for(int i = 0; i < 100; i++) {
		printf("x: %f, y: %f, z: %f\n",
				meshPos[i * 3],
				meshPos[i * 3 + 1],
				meshPos[i * 3 + 2]);
	}
}

static void pressEnter(unsigned char key, int x, int y)
{
	createMesh();
	init();
	display();
}

static void reshapeFunc(int w, int h) {
	glViewport(0, 0, w, h);
	float aspect = (float) w / h;
	if(usePerspective) {
		setPerspective(&projMatrix, 60, aspect, -10, 10);
	} else {
		float cubeLength = 10;
		if(aspect >= 1.0) setOrtho(&projMatrix, -cubeLength * aspect, cubeLength * aspect,
												-cubeLength, cubeLength, -cubeLength, cubeLength);
		else setOrtho(&projMatrix, -cubeLength,  cubeLength, -cubeLength / aspect, cubeLength / aspect,
												-cubeLength, cubeLength);
	}
	glUniformMatrix4fv(projMatrixLoc, 1, GL_TRUE, projMatrix.values);
}

static void init() {
	// Bind vertex array
	glGenVertexArrays(1, va);
	glBindVertexArray(va[0]);
	glGenBuffers(3, bufferId);

	// Bind position buffer
	glBindBuffer(GL_ARRAY_BUFFER, bufferId[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(meshPos), meshPos, GL_STATIC_DRAW);
	glVertexAttribPointer(vertexPositionLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vertexPositionLoc);

	// Bind color buffer
	glBindBuffer(GL_ARRAY_BUFFER, bufferId[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(meshColor), meshColor, GL_STATIC_DRAW);
	glVertexAttribPointer(vertexColLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vertexColLoc);

	// Bind index buffer
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferId[2]);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(meshIndex), meshIndex, GL_STATIC_DRAW);

	// Allow restarts
//	glEnable(GL_PRIMITIVE_RESTART);
//	glPrimitiveRestartIndex(0xFFFF);

}

static void createMesh()
{
	// One index for each buffer
	int columnIndex = 0;
	int bufferIndex = 0;
	int colorIndex = 0;

	float startPointy = 5.0 - ROW_WIDTH;
	for(int i = 0; i < ROWS; i++, startPointy -= ROW_WIDTH)
	{
		float startPointx = -5.0;

		// Calculate the positions of the mesh points
		for(int j = 0; j < COLUMNS + 1; j++, columnIndex++, startPointx += COLUMN_WIDTH)
		{
			// Add two points at a time, to create a vertical line
			meshPos[(columnIndex * 6)] = startPointx;
			meshPos[(columnIndex * 6) + 1] =
			(1.0 / 20.0) * (20.0 + (startPointx * startPointx)
					           + (startPointy * startPointy)
							   - (10 * cos(2 * M_PI * startPointx))
							   + (cos(2 * M_PI * startPointy)));

			meshPos[(columnIndex * 6) + 2] = startPointy;

			int Ztemp = startPointy + ROW_WIDTH;
			meshPos[(columnIndex * 6) + 3] = startPointx;
			meshPos[(columnIndex * 6) + 4] =
			(1.0 / 20.0) * (20 + (startPointx * startPointx)
										   + (Ztemp * Ztemp)
										   - (10 * cos(2 * M_PI * startPointx))
										   + (cos(2 * M_PI * Ztemp)));

			meshPos[(columnIndex * 6) + 5] = startPointy + ROW_WIDTH;

		}

		// Calculate the values for the indexes
		for(int a = 0; a < (COLUMNS + 1) * 2; a++, bufferIndex++)
		{
			meshIndex[bufferIndex + i] = bufferIndex;
		}
		meshIndex[bufferIndex + i] = 0xFFFF;	// Add restart value to the indexes

		// Produce random colors for each of the vertexes
		// Notice how each color is assigned to two vertex
		// That are located at the same place to produce only one color
		for(int k = 0; k < COLUMNS + 1; k++, colorIndex += 2)
		{
			int mirror = ((2 * (COLUMNS + 1)) + 1 + colorIndex);

			meshColor[colorIndex * 3]       =
			meshColor[ mirror * 3]          = rand()/(RAND_MAX*1.0);

			meshColor[(colorIndex * 3) + 1] =
			meshColor[(mirror * 3) + 1]     = rand()/(RAND_MAX*1.0);

			meshColor[(colorIndex * 3) + 2] =
			meshColor[(mirror * 3) + 2]     = rand()/(RAND_MAX*1.0);
		}
 	}

	// Assigning upper-most peaks with a color
	for(int f = 1; f < (COLUMNS + 1) * 2; f += 2){
		meshColor[(f * 3)]     = rand()/(RAND_MAX*1.0);
		meshColor[(f * 3) + 1] = rand()/(RAND_MAX*1.0);
		meshColor[(f * 3) + 2] = rand()/(RAND_MAX*1.0);
	}

	printVertexPositions();

}

static void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(programId);
	glPointSize(5.0);
	glLineWidth(5.0);
	glBindVertexArray(va[0]);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferId[2]);
	glDrawArrays(primitive,0, (((2 * (COLUMNS + 1)) * ROWS) + ROWS - 1));
	glutSwapBuffers();
}

static void timerFunc(int id)
{
	glutTimerFunc(20, timerFunc, id);
	glutPostRedisplay();
}

int main_notmain(int argc, char** argv) {
	setbuf(stdout, NULL);

	srand(time(0));

	glutInit(&argc, argv);
	int w = 600, h = 600;
	int x = (glutGet(GLUT_SCREEN_WIDTH) - w) / 2;
	int y = (glutGet(GLUT_SCREEN_HEIGHT) - h) / 2;


	glutInitWindowSize(w, h);
	glutInitWindowPosition(x, y);
	glutTimerFunc(50, timerFunc, 1);

	glutCreateWindow("Concurso: Mesh 3D");
	glutInitDisplayMode(GLUT_DOUBLE);
	glutDisplayFunc(display);
	glutKeyboardFunc(pressEnter);
	glutReshapeFunc(reshapeFunc);
	glewInit();
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

	modelMatrixLoc    = glGetUniformLocation(programId, "modelMatrix");
	viewMatrixLoc     = glGetUniformLocation(programId, "viewMatrix");
	projMatrixLoc     = glGetUniformLocation(programId, "projMatrix");


}

