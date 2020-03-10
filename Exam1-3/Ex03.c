/*
 * Practica10.c
 *
 *  Created on: 19/09/2016
 *      Author: Cursos
 */

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <Utils.h>
#include <stdio.h>
#include <math.h>

static float *vertexArray;
static float *colorArray;
static int *radiusArray;
static int vertexArrayLength;
static int colorArrayLength;
static int radiusArrayLength;
static GLuint programId;
static GLuint vertexId;
static GLuint buffId[3];
static GLuint vertexPosLoc;
static GLuint colorVertexLoc;
static GLuint scaleInternalLoc;
static GLuint scaleExternalLoc;
static GLuint radiusPosLoc;
int gPicos = 20;
float gRadioExterno = 0.8;
float gRadioInterno = 0.4;
float scaleInternal = 1.0;
float scaleExternal = 1.0;

float example[] = {
    .9, -.9,
    -.9, .9,
    0, .9};

float rad(float deg)
{
    return deg * 3.14159265358979323846 / 180;
}

/**
 * Genera el arrego de vértices y colores
 * @param vertex OUT
 * @param color OUT
 * @param attribArray OUT
 * @param picos
 * @param radioExterior
 * @param radioInterior
 */
void generateVertex(float *vertex, float *color, int *radius, const int picos, const float radioExterior, const float radioInterior)
{
    const float delta = 360.0 / (picos * 2.0);
    vertex[0] = 0.0;
    vertex[1] = 0.0;
    // vertex[2] = 0;
    color[0] = 1.0;
    color[1] = 1.0;
    color[2] = 1.0;

    radius[0];

    for (int i = 1; i < (picos * 2) + 2; i++)
    {
        float r = rad(delta * i);
        // printf("DELTA: %d %f %f\n", i, delta * i, r);
        int vertexIndex = i * 2;
        int colorIndex = i * 3;
        if (i % 2)
        {
            vertex[vertexIndex] = radioInterior * cosf(r);     // X
            vertex[vertexIndex + 1] = radioInterior * sinf(r); // Y
            // vertex[vertexIndex + 2] = 1;
        }
        else
        {
            vertex[vertexIndex] = radioExterior * cosf(r);     // X
            vertex[vertexIndex + 1] = radioExterior * sinf(r); // Y
            // vertex[vertexIndex + 2] = 0;
        }

        radius[i] = i % 2;

        // TODO: Colores
        color[colorIndex] = fabs(sinf(r));             // R
        color[colorIndex + 1] = fabs(cosf(r));         // G
        color[colorIndex + 2] = r >= 3.14159 ? .5 : 0; // B
    }
}

void initShaders()
{
    GLuint vShader = compileShader("../shaders/star.vsh", GL_VERTEX_SHADER);
    if (!shaderCompiled(vShader))
        return;
    GLuint fShader = compileShader("../shaders/color.fsh", GL_FRAGMENT_SHADER);
    if (!shaderCompiled(fShader))
        return;

    printf("Hola\n");

    programId = glCreateProgram();
    glAttachShader(programId, vShader);
    glAttachShader(programId, fShader);
    glLinkProgram(programId);
    vertexPosLoc = glGetAttribLocation(programId, "vertexPosition");
    colorVertexLoc = glGetAttribLocation(programId, "vertexColor");
    radiusPosLoc = glGetAttribLocation(programId, "radiusPos");
    scaleExternalLoc = glGetUniformLocation(programId, "scaleExternal");
    scaleInternalLoc = glGetUniformLocation(programId, "scaleInternal");
    glUseProgram(programId);
}

void createStar()
{
    int picos = gPicos;
    float radioExterior = gRadioExterno;
    float radioInterior = gRadioInterno;
    colorArrayLength = sizeof(float) * (2 * picos * 3) + (2 * 3 * sizeof(float));
    vertexArrayLength = sizeof(float) * (2 * picos * 2) + (2 * 2 * sizeof(float));
    radiusArrayLength = sizeof(int) * (2 * picos) + (2 * sizeof(int));

    vertexArray = (float *)malloc(vertexArrayLength);
    colorArray = (float *)malloc(colorArrayLength);
    radiusArray = (int *)malloc(radiusArrayLength);
    generateVertex(vertexArray, colorArray, radiusArray, picos, radioExterior, radioInterior);

    glGenVertexArrays(1, &vertexId);
    glBindVertexArray(vertexId);

    glGenBuffers(3, buffId);

    glBindBuffer(GL_ARRAY_BUFFER, buffId[0]);
    glBufferData(GL_ARRAY_BUFFER, vertexArrayLength, vertexArray, GL_STATIC_DRAW);
    glEnableVertexAttribArray(vertexPosLoc);
    glVertexAttribPointer(vertexPosLoc, 2, GL_FLOAT, 0, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, buffId[1]);
    glBufferData(GL_ARRAY_BUFFER, colorArrayLength, colorArray, GL_STATIC_DRAW);
    glEnableVertexAttribArray(colorVertexLoc);
    glVertexAttribPointer(colorVertexLoc, 3, GL_FLOAT, 0, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, buffId[2]);
    glBufferData(GL_ARRAY_BUFFER, radiusArrayLength, radiusArray, GL_STATIC_DRAW);
    glEnableVertexAttribArray(radiusPosLoc);
    glVertexAttribPointer(radiusPosLoc, 1, GL_INT, 0, 0, 0);
}

void display()
{
    printf("Display %f %f\n", scaleExternal, scaleInternal);
    glClear(GL_COLOR_BUFFER_BIT);
    glBindVertexArray(vertexId);
    glUniform1f(scaleExternalLoc, scaleExternal);
    glUniform1f(scaleInternalLoc, scaleInternal);

    glDrawArrays(GL_TRIANGLE_FAN, 0, (gPicos * 2) + 2);
    glutSwapBuffers();
}

void onKeyPress(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 97: // A
        scaleExternal -= 0.05;
        break;
    case 98:
        scaleInternal -= 0.05;
        break;
    case 99:
        scaleExternal += 0.05;
        break;
    case 100:
        scaleInternal += 0.05;
        break;
    }

    if(scaleExternal <= 0.0){
        scaleExternal = 0.0;
    }
    if (scaleInternal <= 0.0)
    {
        scaleInternal = 0.0;
    }
    

    glutPostRedisplay();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Extraaulica 1.2");
    glutKeyboardFunc(onKeyPress);
    glutDisplayFunc(display);
    printf("%s\n", glGetString(GL_VERSION));
    glewInit();
    initShaders();
    createStar();
    glClearColor(0.5, 0.5, 0.5, 1.0);
    glutMainLoop();

    return 0;
}
