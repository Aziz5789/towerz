#include "mat4.h"

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