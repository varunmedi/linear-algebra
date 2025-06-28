#pragma once

#include <raylib.h>
#include <tgmath.h>

#include "matrix2.h"

typedef struct {
  float   radius;
  Vector2 center;
  float   theta;
  float   speed;
  bool    stay_centered;
} circle_path;

typedef enum { CIRCLE } path_tag;

typedef union path_data {
  circle_path circle_path;
} path_data;

typedef struct {
  path_tag  tag;
  path_data data;
  Color     color;
} path;

typedef struct {
  Vector2 start, end;
  Color   color;
  float   thickness;
} line;

typedef struct {
  Color line_color, i_vector_color, j_vector_color, eigen_1_color,
      eigen_2_color, text_fg_color, text_bg_color, det_pos_color, det_neg_color,
      origin_color;
} grid_style;

typedef struct {
  Vector2    origin;
  float      line_thickness;
  Color      line_color;
  matrix2    basis;
  int        line_count;
  int        num_paths;
  path*      paths;
  grid_style style;
  int        num_points;
  Vector2*   points;
  float      step;
  Vector2    size;
} grid;

typedef struct {
  grid_style dark;
  grid_style light;
} theme_pack;

extern const theme_pack BUILTIN_THEMES;

void    draw_line(const line l);
void    run_2d_interactive_simul(grid* g, bool show_hud, Font font);
void    update_paths(const grid* const g);
Vector2 translate_x(Vector2 v, double x);
Vector2 translate_y(Vector2 v, double y);
Vector2 translate_xy(Vector2 v, double x, double y);
