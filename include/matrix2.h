#pragma once

#include <raylib.h>

typedef struct {
  Vector2 i, j;
} matrix2;

typedef struct {
  Vector2 eigenvectors[2];
  float   eigenvalues[2];
} eigen;

Vector2 transform(matrix2 T, Vector2 v);
matrix2 rotation_matrix(double theta);
float   matrix2det(matrix2 m);

int     matrix2eigen(matrix2 A, eigen* const out);
matrix2 matrix2mul(matrix2 A, matrix2 B);
