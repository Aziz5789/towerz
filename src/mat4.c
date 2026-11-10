#include "mat4.h"
#include <math.h>

Mat4 mat4_identity(void) {
    Mat4 matrix = {0};
    matrix.m[0] = 1.0f;
    matrix.m[5] = 1.0f;
    matrix.m[10] = 1.0f;
    matrix.m[15] = 1.0f;

    return matrix;
}

Mat4 mat4_translate(float tx, float ty, float tz) {

    Mat4 matrix = mat4_identity();

    matrix.m[12] = tx;
    matrix.m[13] = ty;
    matrix.m[14] = tz;

    return matrix;
}

Mat4 mat4_multiply(Mat4 matrix_a, Mat4 matrix_b) {
    Mat4 result = {0};

    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            float sum = 0;
            for (int k = 0; k < 4; k++) {
                sum += matrix_a.m[k * 4 + row] * matrix_b.m[col * 4 + k];
            }
            result.m[col * 4 + row] = sum;
        }
    }

    return result;
}

Mat4 mat4_rotate_z(float radian) {
    Mat4 matrix = mat4_identity();

    float s = sinf(radian);
    float c = cosf(radian);

    matrix.m[0] = c;
    matrix.m[1] = s;
    matrix.m[4] = -s;
    matrix.m[5] = c;

    return matrix;
}

Mat4 mat4_ortho(float left, float right, float bottom, float top, float near, float far) {
    Mat4 matrix = mat4_identity();

    float right_minus_left = right - left;
    float top_minus_bottom = top - bottom;
    float far_minus_near = far - near;

    matrix.m[0] = 2 / right_minus_left;
    matrix.m[5] = 2 / top_minus_bottom;
    matrix.m[10] = -2 / far_minus_near;
    matrix.m[12] = -(right + left) / right_minus_left;
    matrix.m[13] = -(top + bottom) / top_minus_bottom;
    matrix.m[14] = -(far + near) / far_minus_near;

    return matrix;
}

Mat4 mat4_perspective(float fov, float aspect, float near, float far) {
    Mat4 matrix = {0};

    float f = 1 / tanf(fov / 2);
    float near_minus_far = near - far;

    matrix.m[0] = f / aspect;
    matrix.m[5] = f;
    matrix.m[10] = (far + near) / near_minus_far;
    matrix.m[11] = -1;
    matrix.m[14] = (2 * far * near) / near_minus_far;

    return matrix;
}
