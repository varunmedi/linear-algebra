#include "quad.h"

void DrawQuad(QuadCoords coords, Color color) {
  Vector2 topLeft     = coords.topLeft;
  Vector2 topRight    = coords.topRight;
  Vector2 bottomLeft  = coords.bottomLeft;
  Vector2 bottomRight = coords.bottomRight;

#if defined(SUPPORT_QUADS_DRAW_MODE)
  rlSetTexture(GetShapesTexture().id);
  Rectangle shapeRect = GetShapesTextureRectangle();

  rlBegin(RL_QUADS);

  rlNormal3f(0.0f, 0.0f, 1.0f);
  rlColor4ub(color.r, color.g, color.b, color.a);

  rlTexCoord2f(shapeRect.x / texShapes.width, shapeRect.y / texShapes.height);
  rlVertex2f(topLeft.x, topLeft.y);

  rlTexCoord2f(shapeRect.x / texShapes.width,
               (shapeRect.y + shapeRect.height) / texShapes.height);
  rlVertex2f(bottomLeft.x, bottomLeft.y);

  rlTexCoord2f((shapeRect.x + shapeRect.width) / texShapes.width,
               (shapeRect.y + shapeRect.height) / texShapes.height);
  rlVertex2f(bottomRight.x, bottomRight.y);

  rlTexCoord2f((shapeRect.x + shapeRect.width) / texShapes.width,
               shapeRect.y / texShapes.height);
  rlVertex2f(topRight.x, topRight.y);

  rlEnd();

  rlSetTexture(0);
#else
  rlBegin(RL_TRIANGLES);

  rlColor4ub(color.r, color.g, color.b, color.a);

  rlVertex2f(topLeft.x, topLeft.y);
  rlVertex2f(bottomLeft.x, bottomLeft.y);
  rlVertex2f(topRight.x, topRight.y);

  rlVertex2f(topRight.x, topRight.y);
  rlVertex2f(bottomLeft.x, bottomLeft.y);
  rlVertex2f(bottomRight.x, bottomRight.y);

  rlEnd();
#endif
}
