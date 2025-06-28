#include "grid.h"

#include <math.h>
#include <raylib.h>
#include <rlgl.h>
#include <stdio.h>
#include <string.h>

#include "matrix2.h"
#include "quad.h"
#include "raylib_ext.h"

void draw_line(const line l) {
  DrawLineEx(l.start, l.end, l.thickness, l.color);
}

const theme_pack BUILTIN_THEMES = {
    .dark =
        {
            .i_vector_color = PINK,
            .j_vector_color = PINK,
            .eigen_1_color  = GREEN,
            .eigen_2_color  = GREEN,
            .text_bg_color  = RAYWHITE,
            .text_fg_color  = BLACK,
            .det_neg_color  = ORANGE,
            .det_pos_color  = BLUE,
            .line_color     = RAYWHITE,
            .origin_color   = BLUE,
        },
    .light =
        {
            .i_vector_color = PINK,
            .j_vector_color = PINK,
            .eigen_1_color  = GREEN,
            .eigen_2_color  = GREEN,
            .text_bg_color  = BLACK,
            .text_fg_color  = RAYWHITE,
            .det_neg_color  = ORANGE,
            .det_pos_color  = BLUE,
            .line_color     = BLACK,
            .origin_color   = BLUE,
        },
};

Vector2 translate_x(Vector2 v, double x) { return (Vector2){v.x + x, v.y}; }

Vector2 translate_y(Vector2 v, double y) { return (Vector2){v.x, v.y + y}; }

Vector2 translate_xy(Vector2 v, double x, double y) {
  return translate_y(translate_x(v, x), y);
}

static const Vector2 arrow_p1 = {10, 0};
static const Vector2 arrow_p2 = {-5, -5};
static const Vector2 arrow_p3 = {-5, 5};

static void write_vec(Vector2 v, char* const buf) {
  char _buf[20];
  if (v.x > 0 && v.y > 0 || v.x < 0 && v.y < 0) {
    sprintf(buf, "|%0.2f|\n|%0.2f|", v.x, v.y);
  } else if (v.x < 0) {
    sprintf(_buf, " %0.2f", v.y);
    sprintf(buf, "|%0.2f|\n|%s|", v.x, _buf);
  } else {
    sprintf(_buf, " %0.2f", v.x);
    sprintf(buf, "|%s|\n|%0.2f|", _buf, v.y);
  }
}

static void draw_arrowhead(float x, float y, float scale, double angle_rad,
                           Color color) {
  matrix2 R = rotation_matrix(angle_rad);

  Vector2 p1 = translate_x(translate_y(transform(R, arrow_p1), y), x);
  Vector2 p2 = translate_x(translate_y(transform(R, arrow_p2), y), x);
  Vector2 p3 = translate_x(translate_y(transform(R, arrow_p3), y), x);

  DrawTriangle(p1, p2, p3, color);
}

static void draw_det2(const grid* const g, Font font, bool show_hud) {
  int     scale       = 40;
  float   determinant = matrix2det(g->basis);
  Vector2 i_hat = g->basis.i, j_hat = g->basis.j;
  i_hat.y *= -1;
  j_hat.y *= -1;
  QuadCoords coords = {0};

  coords.bottomLeft  = translate_xy((Vector2){0, 0}, g->origin.x, g->origin.y);
  coords.bottomRight = translate_xy((Vector2){i_hat.x * scale, i_hat.y * scale},
                                    g->origin.x, g->origin.y);
  coords.topLeft     = translate_xy((Vector2){j_hat.x * scale, j_hat.y * scale},
                                    g->origin.x, g->origin.y);
  coords.topRight    = translate_xy((Vector2){i_hat.x * scale + j_hat.x * scale,
                                              j_hat.y * scale + i_hat.y * scale},
                                    g->origin.x, g->origin.y);
  if (determinant < 0) {
    Vector2 tmp     = coords.topLeft;
    coords.topLeft  = coords.topRight;
    coords.topRight = tmp;

    tmp                = coords.bottomLeft;
    coords.bottomLeft  = coords.bottomRight;
    coords.bottomRight = tmp;
  }

  DrawQuad(coords, determinant > 0 ? BLUE : ORANGE);
  char buf[100];

  if (show_hud) {
    sprintf(buf, "det: %0.2f", determinant);
    DrawRectangle((coords.topLeft.x + coords.bottomRight.x) / 2 - 5,
                  (coords.topLeft.y + coords.bottomRight.y) / 2 - 3, 95, 20,
                  g->style.text_bg_color);
    DrawTextEx(font, buf,
               (Vector2){(coords.topLeft.x + coords.bottomRight.x) / 2,
                         (coords.topLeft.y + coords.bottomRight.y) / 2

               },
               16, 0, g->style.text_fg_color);
  }
}

void update_circle_path(circle_path* const c, matrix2 basis, Color color) {
  c->theta += c->speed * 20 * 0.01745329F;
  if (c->stay_centered) {
    c->center.x = (float)GetCorrectedScreenWidth() / 2;
    c->center.y = (float)GetCorrectedScreenHeight() / 2;
  }

  float   r   = c->radius * 40;
  float   x   = r * sin(c->theta);
  float   y   = r * cos(c->theta);
  Vector2 loc = {x, y};

  matrix2 R = basis;
  R.i.y *= -1;
  R.j.x *= -1;

  Vector2 new_loc = translate_xy(transform(R, loc), c->center.x, c->center.y);

  DrawCircle(new_loc.x, new_loc.y, 5, color);
}

void update_paths(const grid* const g) {
  for (int i = 0; i < g->num_paths; i++) {
    switch (g->paths[i].tag) {
      case CIRCLE:
        update_circle_path(&g->paths[i].data.circle_path, g->basis,
                           g->paths[i].color);
        break;
      default:
        break;
    }
  }
}

void run_2d_interactive_simul(grid* g, bool show_hud, Font font) {
  int scale            = 40;
  int line_lower_bound = g->line_count / 2;
  int line_upper_bound = line_lower_bound;
  if (g->line_count % 2 == 1) {
    line_upper_bound += 1;
  }

  Vector2 i_hat = g->basis.i;
  Vector2 j_hat = g->basis.j;
  matrix2 basis = {i_hat, j_hat};

  line i = {.color     = g->style.i_vector_color,
            .start     = {.x = g->origin.x, .y = g->origin.y},
            .end       = {.x = g->origin.x + i_hat.x * scale,
                          .y = g->origin.y - i_hat.y * scale},
            .thickness = g->line_thickness};

  line j = {.color     = g->style.j_vector_color,
            .start     = {.x = g->origin.x, .y = g->origin.y},
            .end       = {.x = g->origin.x + j_hat.x * scale,
                          .y = g->origin.y - j_hat.y * scale},
            .thickness = g->line_thickness};

  double basis_y_len = sqrtl(pow(g->basis.j.x, 2) + pow(g->basis.j.y, 2));
  double basis_x_len = sqrtl(pow(g->basis.i.x, 2) + pow(g->basis.i.y, 2));
  double len         = basis_x_len * line_lower_bound * scale;

  double theta = atan(i_hat.y / i_hat.x);
  double alpha = PI / 2 - atan(j_hat.x / j_hat.y);
  double x     = (len)*cos(theta);
  double y     = (len)*sin(theta);

  float screen_height = GetCorrectedScreenHeight();
  float screen_width  = GetCorrectedScreenHeight();
  update_paths(g);
  for (int i = 0; i < g->num_points; i++) {
    Vector2 point = g->points[i];
    DrawCircle(point.x, point.y, 5, RED);

    if (CheckCollisionPointCircle(GetCorrectedMousePosition(), point, 8)) {
      char txtbuf[100];
      sprintf(txtbuf, "(%0.2f, %0.2f)", (point.x - g->origin.x) / 40,
              -(point.y - g->origin.y) / 40);
      DrawTextEx(font, txtbuf, (Vector2){point.x - 20, point.y - 20}, 16, 0,
                 g->style.text_bg_color);
    }
  }

  for (int i = -line_lower_bound; i < line_upper_bound; i++) {
    line l1 = {
        translate_xy((Vector2){g->origin.x - x, g->origin.y + y},
                     g->basis.j.x * scale * i, -g->basis.j.y * scale * i),
        translate_xy((Vector2){g->origin.x + x, g->origin.y - y},
                     g->basis.j.x * scale * i, -g->basis.j.y * scale * i),
        g->style.line_color, g->line_thickness};
    draw_line(l1);
  }

  double len_p = (basis_y_len * len) / basis_x_len;
  double x_p   = len_p * cos(alpha);
  double y_p   = len_p * sin(alpha);

  for (int i = -line_lower_bound; i < line_upper_bound; i++) {
    line l2 = {
        translate_xy((Vector2){g->origin.x - x_p, g->origin.y + y_p},
                     g->basis.i.x * scale * i, -g->basis.i.y * scale * i),
        translate_xy((Vector2){g->origin.x + x_p, g->origin.y - y_p},
                     g->basis.i.x * scale * i, -g->basis.i.y * scale * i),
        g->style.line_color, g->line_thickness};
    draw_line(l2);
  }

  draw_det2(g, font, show_hud);

  double arrowhead1_angle = theta;
  if (i_hat.x < 0) {
    arrowhead1_angle = PI - theta;
    if (i_hat.y < 0) {
      arrowhead1_angle *= -1;
    }
    if (i_hat.y > 0) {
      arrowhead1_angle = PI + theta;
    }
  }

  double arrowhead2_angle = alpha;
  if (j_hat.y < 0) {
    arrowhead2_angle *= -1;
    if (j_hat.x > 0) {
      arrowhead2_angle = PI + alpha;
    }
    if (j_hat.x < 0) {
      arrowhead2_angle = PI + alpha;
    }
  }

  draw_line(i);
  draw_line(j);
  Rectangle irec = {i.end.x, i.end.y - 25, 20, 20};
  Rectangle jrec = {j.end.x, j.end.y - 25, 20, 20};

  draw_arrowhead(g->origin.x + i_hat.x * scale, g->origin.y - i_hat.y * scale,
                 1, arrowhead1_angle, PINK);
  draw_arrowhead(g->origin.x + j_hat.x * scale, g->origin.y - j_hat.y * scale,
                 1, arrowhead2_angle, PINK);

  if (show_hud) {
    DrawRectangleRec(irec, g->style.text_bg_color);
    DrawTextEx(font, "i", (Vector2){irec.x + 5, irec.y + 2}, 16, 0,
               g->style.text_fg_color);
    DrawRectangleRec(jrec, g->style.text_bg_color);
    DrawTextEx(font, "j", (Vector2){jrec.x + 5, jrec.y + 2}, 16, 0,
               g->style.text_fg_color);

    if (CheckCollisionPointRec(GetCorrectedMousePosition(), irec)) {
      Rectangle info = {i.end.x, i.end.y - 5, 75, 40};
      DrawRectangleRec(info, g->style.text_bg_color);
      char buf[100];
      write_vec(g->basis.i, buf);
      DrawTextEx(font, buf, (Vector2){info.x + 5, info.y + 2}, 16, 0,
                 g->style.text_fg_color);
    }

    if (CheckCollisionPointRec(GetCorrectedMousePosition(), jrec)) {
      Rectangle info = {j.end.x, j.end.y - 5, 75, 40};
      DrawRectangleRec(info, g->style.text_bg_color);
      char buf[100];
      write_vec(g->basis.j, buf);
      DrawTextEx(font, buf, (Vector2){info.x + 5, info.y + 2}, 16, 0,
                 g->style.text_fg_color);
    }
  }

  eigen e;
  matrix2eigen(g->basis, &e);
  char buf[100];
  line ev1 = {
      .start     = translate_xy((Vector2){0, 0}, g->origin.x, g->origin.y),
      .end       = {g->origin.x + scale * e.eigenvectors[0].x,
                    g->origin.y + scale * e.eigenvectors[0].y * -1},
      .color     = g->style.eigen_1_color,
      .thickness = 2,
  };

  double ev1_angle = atan(e.eigenvectors[0].y / e.eigenvectors[0].x);

  if (e.eigenvectors[0].x < 0) {
    ev1_angle += PI;
  }

  draw_line(ev1);
  draw_arrowhead(ev1.end.x, ev1.end.y, 1, ev1_angle, g->style.eigen_1_color);

  if (show_hud) {
    if (!isnan(e.eigenvectors[0].x) && !isnan(e.eigenvectors[0].y)) {
      sprintf(buf, "u (λ=%0.2f)", e.eigenvalues[0]);
      Rectangle rec = {ev1.end.x - 5, ev1.end.y - 23, 88, 20};
      DrawRectangleRec(rec, g->style.text_bg_color);
      DrawTextEx(font, buf, (Vector2){ev1.end.x, ev1.end.y - 20}, 16, 0,
                 g->style.text_fg_color);
      memset(buf, 0, 100);
      if (CheckCollisionPointRec(GetCorrectedMousePosition(), rec)) {
        Rectangle info = {ev1.end.x - 5, ev1.end.y - 5, 88, 40};

        write_vec(e.eigenvectors[0], buf);

        DrawRectangleRec(info, g->style.text_bg_color);
        DrawTextEx(font, buf, (Vector2){info.x + 5, info.y + 5}, 16, 0,
                   g->style.text_fg_color);
      }
    }
  }

  line ev2 = {
      .start     = translate_xy((Vector2){0, 0}, g->origin.x, g->origin.y),
      .end       = {g->origin.x + scale * e.eigenvectors[1].x,
                    g->origin.y + scale * e.eigenvectors[1].y * -1},
      .color     = g->style.eigen_2_color,
      .thickness = 2,
  };

  double ev2_angle = atan(e.eigenvectors[1].y / e.eigenvectors[1].x);

  if (e.eigenvectors[1].x < 0) {
    ev2_angle += PI;
  }
  draw_arrowhead(ev2.end.x, ev2.end.y, 1, ev2_angle, g->style.eigen_2_color);

  if (show_hud) {
    memset(buf, 0, 100);
    if (!isnan(e.eigenvectors[1].x) && !isnan(e.eigenvectors[1].y)) {
      sprintf(buf, "v (λ=%0.2f)", e.eigenvalues[1]);
      Rectangle rec = {ev2.end.x - 5, ev2.end.y - 23, 88, 20};
      DrawRectangleRec(rec, g->style.text_bg_color);
      DrawTextEx(font, buf, (Vector2){ev2.end.x, ev2.end.y - 20}, 16, 0,
                 g->style.text_fg_color);

      memset(buf, 0, 100);
      if (CheckCollisionPointRec(GetCorrectedMousePosition(), rec)) {
        Rectangle info = {ev2.end.x - 5, ev2.end.y - 5, 88, 40};

        write_vec(e.eigenvectors[1], buf);

        DrawRectangleRec(info, g->style.text_bg_color);
        DrawTextEx(font, buf, (Vector2){info.x + 5, info.y + 5}, 16, 0,
                   g->style.text_fg_color);
      }
    }
  }

  draw_line(ev2);

  DrawCircle(g->origin.x, g->origin.y, 4, g->style.origin_color);
  if (show_hud) {
    DrawTextEx(font, "origin", (Vector2){g->origin.x + 5, g->origin.y + 10}, 16,
               0, g->style.text_bg_color);
  }
}
