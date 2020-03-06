/*
 * examen.c
 *
 *  Created on: 02/03/2020
 *      Author: Dario
 */
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "Utils/Utils.h"
#include "Transforms/Transforms.h"
#include <stdio.h>

static GLuint vertexColLoc, vertexPositionLoc, dimensions, programId, isRgb;
static void display();
int globaIsRGB = GL_FALSE;

void pressEnter(unsigned char key, int x, int y)
{
	globaIsRGB ^= 1;
	display();
}

static void initShaders() {
	GLuint vShaderId = compileShader("shaders/vertexPosition.vsh", GL_VERTEX_SHADER, GL_FALSE);
	if(!shaderCompiled(vShaderId)) return;

	GLuint fShaderId = compileShader("shaders/fractal.fsh", GL_FRAGMENT_SHADER, GL_TRUE);
	if(!shaderCompiled(fShaderId)) return;

	programId = glCreateProgram();
	glAttachShader(programId, vShaderId);
	glAttachShader(programId, fShaderId);
	glLinkProgram(programId);

	vertexPositionLoc = glGetAttribLocation (programId, "vertexPosition");	// Vertex Shader
	vertexColLoc      = glGetAttribLocation (programId, "vertexColor");		// Vertex shader
	dimensions        = glGetUniformLocation(programId, "dimensions");		// Vertex Shader

	isRgb             = glGetUniformLocation(programId, "isRGB");			// Fragment shader

}

static void init() {
	float pos[] = { -0.98, 0.98,    -0.98, -0.98,    0.98, 0.98,    0.98, -0.98};
	float color[12] = {};

	GLuint vertexArrayId;
	glGenVertexArrays(1, &vertexArrayId);
	glBindVertexArray(vertexArrayId);
	GLuint bufferId1, bufferId2;

	glGenBuffers(1, &bufferId1);
	glBindBuffer(GL_ARRAY_BUFFER, bufferId1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pos), pos, GL_STATIC_DRAW);
	glVertexAttribPointer(vertexPositionLoc, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vertexPositionLoc);

	glGenBuffers(1, &bufferId2);
	glBindBuffer(GL_ARRAY_BUFFER, bufferId2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);
	glVertexAttribPointer(vertexColLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vertexColLoc);

}

static void display() {
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(programId);
	glUniform2i(dimensions, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));

	glUniform1i(isRgb, globaIsRGB);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glutSwapBuffers();
}


int main_1_1(int argc, char** argv) {
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
	init();

	glutMainLoop();
	return 0;
}
