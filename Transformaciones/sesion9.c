///*
// * sesion9.c
// *
// *  Created on: 23/02/2020
// *      Author: Dario
// */
//
//
//#include <GL/glew.h>
//#include <GL/freeglut.h>
//#include "Utils/Utils.h"
//#include "Transforms/Transforms.h"
//
//static GLuint squareColorLoc, vertexPositionLoc, csMatrixLoc, programId;
//
//static void initShaders() {
//	GLuint vShaderId = compileShader("shaders/vertexPositionCS.vsh", GL_VERTEX_SHADER);
//	if(!shaderCompiled(vShaderId)) return;
//	GLuint fShaderId = compileShader("shaders/modelColor.fsh", GL_FRAGMENT_SHADER);
//	if(!shaderCompiled(fShaderId)) return;
//	programId = glCreateProgram();
//	glAttachShader(programId, vShaderId);
//	glAttachShader(programId, fShaderId);
//	glLinkProgram(programId);
//	vertexPositionLoc = glGetAttribLocation (programId, "vertexPosition");	// Vertex Shader
//	csMatrixLoc       = glGetUniformLocation(programId, "csMatrix");		// Vertex Shader
//	squareColorLoc    = glGetUniformLocation(programId, "squareColor");		// Fragment Shader
//}
//
//static void init() {
//	float pos[] = { -0.2, 0.2,    -0.2, -0.2,    0.2, 0.2,    0.2, -0.2 };
//	GLuint vertexArrayId;
//	glGenVertexArrays(1, &vertexArrayId);
//	glBindVertexArray(vertexArrayId);
//	GLuint bufferId;
//	glGenBuffers(1, &bufferId);
//	glBindBuffer(GL_ARRAY_BUFFER, bufferId);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(pos), pos, GL_STATIC_DRAW);
//	glVertexAttribPointer(vertexPositionLoc, 2, GL_FLOAT, GL_FALSE, 0, 0);
//	glEnableVertexAttribArray(vertexPositionLoc);
//}
//
//static void display() {
//	glClear(GL_COLOR_BUFFER_BIT);
//	glUseProgram(programId);
//	Mat4 csMatrix;
//
//	mIdentity(&csMatrix);
//	glUniformMatrix4fv(csMatrixLoc, 1, GL_TRUE, csMatrix.values);
//	glUniform3f(squareColorLoc, 0.8, 0.6, 0.2);
//	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
//
//	static float angle1 = 0, angle2 = 0, angle3 = 0;
//	rotateZ(&csMatrix, angle1);
//	translate(&csMatrix, 0.6, 0.0, 0.0);
//	rotateZ(&csMatrix, angle2);
//	glUniformMatrix4fv(csMatrixLoc, 1, GL_TRUE, csMatrix.values);
//	glUniform3f(squareColorLoc, 0.5, 0.1, 0.6);
//	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
//
//	translate(&csMatrix, 0.0, 0.6, 0.0);
//	rotateZ(&csMatrix, -angle3);
//	scale(&csMatrix, 0.5, 0.5, 1.0);
//	glUniformMatrix4fv(csMatrixLoc, 1, GL_TRUE, csMatrix.values);
//	glUniform3f(squareColorLoc, 1.0, 0.0, 0.0);
//	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
//	angle1 += 0.8;
//	angle2 += 2.0;
//	angle3 += 20.0;
//
//	static float angle = -45;
//
//	mIdentity(&csMatrix);
//	translate(&csMatrix, -0.6, 0.0, 0.0);
//	rotateZ(&csMatrix, -angle);
//	glUniformMatrix4fv(csMatrixLoc, 1, GL_TRUE, csMatrix.values);
//	glUniform3f(squareColorLoc, 1.0, 0.5, 0.8);
//	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
//
//	mIdentity(&csMatrix);
//	translate(&csMatrix, 0.0, 0.6, 0.0);
//	rotateZ(&csMatrix, angle);
//	scale(&csMatrix, 1.2, 0.7, 1.0);
//	glUniformMatrix4fv(csMatrixLoc, 1, GL_TRUE, csMatrix.values);
//	glUniform3f(squareColorLoc, 0.9, 1.0, 0.4);
//	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
//
//	mIdentity(&csMatrix);
//	translate(&csMatrix, 0.0, -0.6, 0.0);
//	scale(&csMatrix, 1.2, 0.7, 1.0);
//	rotateZ(&csMatrix, angle);
//	glUniformMatrix4fv(csMatrixLoc, 1, GL_TRUE, csMatrix.values);
//	glUniform3f(squareColorLoc, 0.2, 0.8, 1.0);
//	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
//
//	angle += 0.5;
//	if(angle >= 360.0) angle -= 360.0;
//
//	glutSwapBuffers();
//}
//
//static void timerFunc(int id) {
//	glutPostRedisplay();
//	glutTimerFunc(10, timerFunc, 1);
//}
//
//int main_sesion9(int argc, char** argv) {
//	glutInit(&argc, argv);
//	int w = 600, h = 600;
//	int x = (glutGet(GLUT_SCREEN_WIDTH) - w) / 2;
//	int y = (glutGet(GLUT_SCREEN_HEIGHT) - h) / 2;
//	glutInitWindowSize(w, h);
//	glutInitWindowPosition(x, y);
//	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
//	glutCreateWindow("Testing transforms");
//	glutDisplayFunc(display);
//	glutTimerFunc(10, timerFunc, 1);
//	glewInit();
//	glClearColor(0.95, 0.95, 1.0, 1.0);
//	initShaders();
//	init();
//	glutMainLoop();
//	return 0;
//}
//
//
//
//
