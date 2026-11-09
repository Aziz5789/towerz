#ifndef MAT4_H
#define MAT4_H

typedef struct { float m[16];} Mat4;

Mat4 mat4_identity(void);
Mat4 mat4_translate(float tx, float ty, float tz);
Mat4 mat4_multiply(Mat4 matrix_a, Mat4 matrix_b);


#endif
