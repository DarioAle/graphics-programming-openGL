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

// Input parameters
#define ROWS    8
#define COLUMNS 8

#define ROW_WIDTH (2.0 / ROWS)       // Whole normalized squared is two units wide
#define COLUMN_WIDTH (2.0 / COLUMNS) // Whole normalized squared is two units high

// Initialize three buffers
GLushort meshIndex    [((2 * (COLUMNS + 1)) * ROWS) + ROWS - 1] = {};
static float meshPos  [((2 * (COLUMNS + 1)) * ROWS) * 2]        = {};
static float meshColor[((2 * (COLUMNS + 1)) * ROWS) * 3]        = {};

// buffer id and attributes location
static GLuint vertexColLoc, vertexPositionLoc,programId;
static GLuint va[1] = {};
static GLuint bufferId[3] = {};

// Function definitions
static void display();
static void initShaders();
static void createMesh();
static void init();

GLuint primitive = GL_LINE_STRIP;

static void pressEnter(unsigned char key, int x, int y)
{
//	primitive = primitive == GL_TRIANGLE_STRIP ? GL_LINE_STRIP : GL_TRIANGLE_STRIP;
	createMesh();
	init();
	display();
}

static void init() {
	// Bind vertex array
	glGenVertexArrays(1, va);
	glBindVertexArray(va[0]);
	glGenBuffers(3, bufferId);

	// Bind position buffer
	glBindBuffer(GL_ARRAY_BUFFER, bufferId[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(meshPos), meshPos, GL_STATIC_DRAW);
	glVertexAttribPointer(vertexPositionLoc, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vertexPositionLoc);

	// Bind color buffer
	glBindBuffer(GL_ARRAY_BUFFER, bufferId[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(meshColor), meshColor, GL_STATIC_DRAW);
	glVertexAttribPointer(vertexColLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vertexColLoc);

	// Bind index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferId[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(meshIndex), meshIndex, GL_STATIC_DRAW);

	// Allow restarts
	glEnable(GL_PRIMITIVE_RESTART);
	glPrimitiveRestartIndex(0xFFFF);

}

static void createMesh()
{
	float startPointy = 1.0 - ROW_WIDTH;
	int columnIndex = 0;
	int bufferIndex = 0;
	int colorIndex = 0;

	for(int i = 0; i < ROWS; i++, startPointy -= ROW_WIDTH)
	{
		float startPointx = -1.0;
		// Calculate the positions of the mesh points
		for(int j = 0; j < COLUMNS + 1; j++, columnIndex++, startPointx += COLUMN_WIDTH)
		{
			// Add two points at a time, to create a vertical line
			meshPos[(columnIndex * 4)] = startPointx;
			meshPos[(columnIndex * 4) + 1] = startPointy;

			meshPos[(columnIndex * 4) + 2] = startPointx;
			meshPos[(columnIndex * 4) + 3] = startPointy + ROW_WIDTH;
		}

		// Calculate the values for the indexes
		for(int a = 0; a < (COLUMNS + 1) * 2; a++, bufferIndex++)
		{
			meshIndex[bufferIndex + i] = bufferIndex;
		}
		meshIndex[bufferIndex + i] = 0xFFFF;	// Add restart value to the indexes
 	}


	float tempColor  [((COLUMNS + 1) * (ROWS + 1)) * 3] = {};
	for(int k = 0; k < (COLUMNS + 1) * (ROWS + 1) * 3; k++, colorIndex++)
	{
		tempColor[k] = rand()/(RAND_MAX*1.0);
	}

	int increment = 0;
	for(int last = 0; last < (((COLUMNS + 1)) * (ROWS)) ; last++)
	{
		int mirror = ((2 * (COLUMNS + 1)) + 1+ increment);

		meshColor[(increment) * 3] =
		meshColor[ mirror * 3] =
				tempColor[(last * 3)];

		meshColor[((increment) * 3) + 1] =
		meshColor[ (mirror * 3) + 1] =
				tempColor[(last * 3) + 1];

		meshColor[((increment) * 3) + 2] =
		meshColor[ (mirror * 3) + 2] =
				tempColor[(last * 3) + 2];
		increment += 2;
	}

	// fillin upper peaks
	for(int desperate = 1; desperate < (COLUMNS + 1) * 2;desperate += 2){
		meshColor[(desperate * 3)] = rand()/(RAND_MAX*1.0);
		meshColor[(desperate * 3) + 1] = rand()/(RAND_MAX*1.0);
		meshColor[(desperate * 3) + 2] = rand()/(RAND_MAX*1.0);
	}

}

static void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(programId);
	glPointSize(5.0);
	glLineWidth(5.0);
	glBindVertexArray(va[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferId[2]);
	glDrawElements(primitive,(((2 * (COLUMNS + 1)) * ROWS) + ROWS - 1), GL_UNSIGNED_SHORT, 0);
	glutSwapBuffers();
}

int main(int argc, char** argv) {
	setbuf(stdout, NULL);

	srand(time(0));

	glutInit(&argc, argv);
	int w = 600, h = 600;
	int x = (glutGet(GLUT_SCREEN_WIDTH) - w) / 2;
	int y = (glutGet(GLUT_SCREEN_HEIGHT) - h) / 2;


	glutInitWindowSize(w, h);
	glutInitWindowPosition(x, y);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);

	glutCreateWindow("Exam 1.4: Color mesh");
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
