#include "matrix.h"

#define PI 3.1415926f
#define normalize(x, y, z)                  \
{                                               \
        float norm = 1.0f / sqrt(x*x+y*y+z*z);  \
        x *= norm; y *= norm; z *= norm;        \
}

#define I(_i, _j) ((_j)+4*(_i))

void matrixSetIdentityM(float *m) {
    memset((void *) m, 0, 16 * sizeof(float));
    m[0] = m[5] = m[10] = m[15] = 1.0f;
}

void matrixSetRotateM(float *m, float a, float x, float y, float z) {
    float s, c;

    memset((void *) m, 0, 15 * sizeof(float));
    m[15] = 1.0f;

    a *= PI / 180.0f;
    s = sin(a);
    c = cos(a);

    if (1.0f == x && 0.0f == y && 0.0f == z) {
        m[5] = c;
        m[10] = c;
        m[6] = s;
        m[9] = -s;
        m[0] = 1;
    } else if (0.0f == x && 1.0f == y && 0.0f == z) {
        m[0] = c;
        m[10] = c;
        m[8] = s;
        m[2] = -s;
        m[5] = 1;
    } else if (0.0f == x && 0.0f == y && 1.0f == z) {
        m[0] = c;
        m[5] = c;
        m[1] = s;
        m[4] = -s;
        m[10] = 1;
    } else {
        normalize(x, y, z);
        float nc = 1.0f - c;
        float xy = x * y;
        float yz = y * z;
        float zx = z * x;
        float xs = x * s;
        float ys = y * s;
        float zs = z * s;
        m[0] = x * x * nc + c;
        m[4] = xy * nc - zs;
        m[8] = zx * nc + ys;
        m[1] = xy * nc + zs;
        m[5] = y * y * nc + c;
        m[9] = yz * nc - xs;
        m[2] = zx * nc - ys;
        m[6] = yz * nc + xs;
        m[10] = z * z * nc + c;
    }
}

void matrixMultiplyMM(float *m, float *lhs, float *rhs) {
    float t[16];
    for (int i = 0; i < 4; i++) {
        register const float rhs_i0 = rhs[I(i, 0)];
        register float ri0 = lhs[I(0, 0)] * rhs_i0;
        register float ri1 = lhs[I(0, 1)] * rhs_i0;
        register float ri2 = lhs[I(0, 2)] * rhs_i0;
        register float ri3 = lhs[I(0, 3)] * rhs_i0;
        for (int j = 1; j < 4; j++) {
            register const float rhs_ij = rhs[I(i, j)];
            ri0 += lhs[I(j, 0)] * rhs_ij;
            ri1 += lhs[I(j, 1)] * rhs_ij;
            ri2 += lhs[I(j, 2)] * rhs_ij;
            ri3 += lhs[I(j, 3)] * rhs_ij;
        }
        t[I(i, 0)] = ri0;
        t[I(i, 1)] = ri1;
        t[I(i, 2)] = ri2;
        t[I(i, 3)] = ri3;
    }
    memcpy(m, t, sizeof(t));
}

void matrixScaleM(float *m, float x, float y, float z) {
    for (int i = 0; i < 4; i++) {
        m[i] *= x;
        m[4 + i] *= y;
        m[8 + i] *= z;
    }
}

void matrixTranslateM(float *m, float x, float y, float z) {
    for (int i = 0; i < 4; i++) {
        m[12 + i] += m[i] * x + m[4 + i] * y + m[8 + i] * z;
    }
}

void getTranslateMatrix(float *m, float x, float y, float z) {
    matrixSetIdentityM(m);

    m[3] = x;
    m[7] = y;
    m[11] = z;
}

void matrixRotateM(float *m, float a, float x, float y, float z) {
    float rot[16], res[16];
    matrixSetRotateM(rot, a, x, y, z);
    matrixMultiplyMM(res, m, rot);
    memcpy(m, res, 16 * sizeof(float));
}

void matrixLookAtM(float *m,
                   float eyeX, float eyeY, float eyeZ,
                   float cenX, float cenY, float cenZ,
                   float upX, float upY, float upZ) {
    float fx = cenX - eyeX;
    float fy = cenY - eyeY;
    float fz = cenZ - eyeZ;
    normalize(fx, fy, fz);
    float sx = fy * upZ - fz * upY;
    float sy = fz * upX - fx * upZ;
    float sz = fx * upY - fy * upX;
    normalize(sx, sy, sz);
    float ux = sy * fz - sz * fy;
    float uy = sz * fx - sx * fz;
    float uz = sx * fy - sy * fx;

    m[0] = sx;
    m[1] = ux;
    m[2] = -fx;
    m[3] = 0.0f;
    m[4] = sy;
    m[5] = uy;
    m[6] = -fy;
    m[7] = 0.0f;
    m[8] = sz;
    m[9] = uz;
    m[10] = -fz;
    m[11] = 0.0f;
    m[12] = 0.0f;
    m[13] = 0.0f;
    m[14] = 0.0f;
    m[15] = 1.0f;
    matrixTranslateM(m, -eyeX, -eyeY, -eyeZ);
}


void matrixMakePerspective(float *m, float fovyRadians, float aspect, float nearZ, float farZ) {
    float cotan = 1.0f / tanf(fovyRadians / 2.0f);

    m[0] = cotan / aspect;
    m[1] = 0.0f;
    m[2] = 0.0f;
    m[3] = 0.0f;
    m[4] = 0.0f;
    m[5] = cotan;
    m[6] = 0.0f;
    m[7] = 0.0f;
    m[8] = 0.0f;
    m[9] = 0.0f;
    m[10] = (farZ + nearZ) / (nearZ - farZ);
    m[11] = -1.0f;
    m[12] = 0.0f;
    m[13] = 0.0f;
    m[14] = (2.0f * farZ * nearZ) / (nearZ - farZ);
    m[15] = 0.0f;
}

void matrixFrustumM(float *m, float left, float right, float bottom, float top, float near, float far) {
    float r_width = 1.0f / (right - left);
    float r_height = 1.0f / (top - bottom);
    float r_depth = 1.0f / (near - far);
    float x = 2.0f * (near * r_width);
    float y = 2.0f * (near * r_height);
    float A = 2.0f * ((right + left) * r_width);
    float B = (top + bottom) * r_height;
    float C = (far + near) * r_depth;
    float D = 2.0f * (far * near * r_depth);

    memset((void *) m, 0, 16 * sizeof(float));
    m[0] = x;
    m[5] = y;
    m[8] = A;
    m[9] = B;
    m[10] = C;
    m[14] = D;
    m[11] = -1.0f;
}

void rotateXd(float *out, float deg) {
    float rad = deg2rad(deg);
    rotateX(out, rad);
}

void rotateX(float *out, float rad) {
    float s = sinf(rad),
            c = cosf(rad),
            a10 = out[4],
            a11 = out[5],
            a12 = out[6],
            a13 = out[7],
            a20 = out[8],
            a21 = out[9],
            a22 = out[10],
            a23 = out[11];
    out[4] = a10 * c + a20 * s;
    out[5] = a11 * c + a21 * s;
    out[6] = a12 * c + a22 * s;
    out[7] = a13 * c + a23 * s;
    out[8] = a20 * c - a10 * s;
    out[9] = a21 * c - a11 * s;
    out[10] = a22 * c - a12 * s;
    out[11] = a23 * c - a13 * s;
}

void rotateYd(float *out, float deg) {
    float rad = deg2rad(deg);
    rotateY(out, rad);
}

void rotateY(float *out, float rad) {
    float s = sinf(rad),
            c = cosf(rad),
            a00 = out[0],
            a01 = out[1],
            a02 = out[2],
            a03 = out[3],
            a20 = out[8],
            a21 = out[9],
            a22 = out[10],
            a23 = out[11];
    out[0] = a00 * c - a20 * s;
    out[1] = a01 * c - a21 * s;
    out[2] = a02 * c - a22 * s;
    out[3] = a03 * c - a23 * s;
    out[8] = a00 * s + a20 * c;
    out[9] = a01 * s + a21 * c;
    out[10] = a02 * s + a22 * c;
    out[11] = a03 * s + a23 * c;
}

void rotateZd(float *out, float deg) {
    float rad = deg2rad(deg);
    rotateZ(out, rad);
}

void rotateZ(float *out, float rad) {
    float s = sinf(rad),
            c = cosf(rad),
            a00 = out[0],
            a01 = out[1],
            a02 = out[2],
            a03 = out[3],
            a10 = out[4],
            a11 = out[5],
            a12 = out[6],
            a13 = out[7];
    out[0] = a00 * c + a10 * s;
    out[1] = a01 * c + a11 * s;
    out[2] = a02 * c + a12 * s;
    out[3] = a03 * c + a13 * s;
    out[4] = a10 * c - a00 * s;
    out[5] = a11 * c - a01 * s;
    out[6] = a12 * c - a02 * s;
    out[7] = a13 * c - a03 * s;
}

void perspective(float fovy, float aspect, float near, float far, float *out) {
    float f = (float) (1.0 / tanf((float) (fovy * PI / 360))), nf = 1 / (near - far);
    out[0] = f / aspect;
    out[1] = 0;
    out[2] = 0;
    out[3] = 0;
    out[4] = 0;
    out[5] = f;
    out[6] = 0;
    out[7] = 0;
    out[8] = 0;
    out[9] = 0;
    out[10] = (far + near) * nf;
    out[11] = -1;
    out[12] = 0;
    out[13] = 0;
    out[14] = (2 * far * near) * nf;
    out[15] = 0;
}