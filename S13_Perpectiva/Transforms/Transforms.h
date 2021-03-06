/*
 * Transforms.h
 *
 *  Created on: 17 feb. 2020
 *      Author: Cursos
 */
#ifndef TRANSFORMS_H_
#define TRANSFORMS_H_

#include "Mat4.h"
#define DEGREES_2_RADIANS(x) (M_PI * x / 180)

void translate(Mat4*, float, float, float);
void rotateX(Mat4*, float);
void rotateY(Mat4*, float);
void rotateZ(Mat4*, float);
void scale (Mat4*, float, float, float);
void loadIdentity(Mat4*);
void pushMatrix(Mat4*);
void popMatrix(Mat4*);
void setOrtho(Mat4*, float, float, float, float, float, float);
void setPerspective(Mat4*, float, float, float, float);

#endif
