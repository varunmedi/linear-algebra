#pragma once

#include <raylib.h>
#include <rlgl.h>

typedef struct {
  Vector2 topLeft, topRight, bottomLeft, bottomRight;
} QuadCoords;

void DrawQuad(QuadCoords coords, Color color);
