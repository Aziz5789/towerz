#ifndef MAT4_H
#define MAT4_H

typedef struct { float m[16];} Mat4;

Mat4 mat4_identity(void);
Mat4 mat4_translate(float tx, float ty, float tz);
Mat4 mat4_multiply(Mat4 matrix_a, Mat4 matrix_b);
Mat4 mat4_rotate_z(float radian);
Mat4 mat4_rotate_y(float radian);
Mat4 mat4_rotate_x(float radian);
Mat4 mat4_ortho(float left, float right, float bottom, float top, float near, float far);
Mat4 mat4_perspective(float fov, float aspect, float near, float far);

#endif
