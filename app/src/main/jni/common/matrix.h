#ifndef MATRIX_H
#define MATRIX_H

#include <math.h>
#include <stdlib.h>
#include <string>

#define PI 3.1415926f


void matrixSetIdentityM(float *m);

void matrixSetRotateM(float *m, float a, float x, float y, float z);

void matrixMultiplyMM(float *m, float *lhs, float *rhs);

void matrixScaleM(float *m, float x, float y, float z);

void matrixTranslateM(float *m, float x, float y, float z);

void matrixRotateM(float *m, float a, float x, float y, float z);

void matrixLookAtM(float *m, float eyeX, float eyeY, float eyeZ, float cenX,
                   float cenY, float cenZ, float upX, float upY, float upZ);

void matrixFrustumM(float *m, float left, float right, float bottom, float top, float near, float far);

void matrixMakePerspective(float *m, float fovyRadians, float aspect, float nearZ, float farZ);

void getTranslateMatrix(float *m, float x, float y, float z);

void rotateX(float *out, float rad);

void rotateY(float *out, float rad);

void rotateZ(float *out, float rad);

void rotateXd(float *out, float deg);

void rotateYd(float *out, float deg);

void rotateZd(float *out, float deg);

void perspective(float fovy, float aspect, float near, float far, float *out);

inline float deg2rad(float deg) {
    return deg * PI / 180.0f;
}

#endif // MATRIX_H
