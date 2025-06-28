#include "matrix2.h"

#include <stdlib.h>
#include <tgmath.h>

Vector2 transform(matrix2 T, Vector2 v) {
  double x = (v.x * T.i.x) + (v.y * T.j.x);
  double y = (v.x * T.i.y) + (v.y * T.j.y);

  return (Vector2){.x = x, .y = y};
}

matrix2 matrix2mul(matrix2 A, matrix2 B) {
  float a = A.i.x, b = A.j.x, c = A.i.y, d = A.j.y;

  float e = B.i.x, f = B.j.x, g = B.i.y, h = B.j.y;

  return (matrix2){{a * e + b * g, a * f + b * h},
                   {c * e + c * g, d * f + d * h}};
}

matrix2 rotation_matrix(double theta) {
  return (matrix2){.i = {cos(theta), -sin(theta)},
                   .j = {sin(theta), cos(theta)}};
}

float matrix2det(matrix2 m) { return m.i.x * m.j.y - m.j.x * m.i.y; }

// Reference:
// https://people.math.harvard.edu/~knill/teaching/math21b2004/exhibits/2dmatrices/index.html
int matrix2eigen(matrix2 A, eigen* const out) {
  float a = A.i.x, b = A.j.x, c = A.i.y, d = A.j.y;

  float T = a + d;          // trace
  float D = a * d - b * c;  // determinant

  float lambda1 /* first eigenvalue */  = (T - sqrtf(pow(T, 2) - 4 * D)) / 2;
  float lambda2 /* second eigenvalue */ = (T + sqrtf(pow(T, 2) - 4 * D)) / 2;

  out->eigenvalues[0] = lambda1;
  out->eigenvalues[1] = lambda2;

  if (b == 0 && c == 0) {
    out->eigenvectors[0] = (Vector2){1, 0};
    out->eigenvectors[1] = (Vector2){0, 1};
  } else if (c != 0) {
    out->eigenvectors[0] = (Vector2){lambda1 - d, c};
    out->eigenvectors[1] = (Vector2){lambda2 - d, c};
  } else if (b != 0) {
    out->eigenvectors[0] = (Vector2){b, lambda1 - a};
    out->eigenvectors[1] = (Vector2){b, lambda2 - a};
  }

  return 0;
}
