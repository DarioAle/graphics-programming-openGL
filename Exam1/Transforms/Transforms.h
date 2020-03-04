/*
 * Transforms.h
 *
 *  Created on: 17 feb. 2020
 *      Author: Cursos
 */
#ifndef TRANSFORMS_H_
#define TRANSFORMS_H_

#include "Mat4.h"

void translate(Mat4*, float, float, float);
void rotateX(Mat4*, float);
void rotateY(Mat4*, float);
void rotateZ(Mat4*, float);
void scale (Mat4*, float, float, float);
void setOrtho(Mat4* m, float, float, float, float, float, float);

#endif
