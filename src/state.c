#include "state.h"

#include <string.h>

#include "grid.h"
#include "raylib.h"
#include "raylib_ext.h"

void run_state(state* s) {
  switch (s->scene.tag) {
    case SCENE_2D_INTERACTIVE_SIMUL:
      run_2d_interactive_simul(s->scene.scene_data.simul_2d.g,
                               s->scene.scene_data.simul_2d.show_hud, s->font);
      break;
    default:
      break;
  }
}

void toggle_show_settings(state* const s) {
  s->show_settings = !s->show_settings;
}

static inline bool shift_pressed() {
  return IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT);
}

static int round_to_multiple(int n, int multiple) {
  int a = (n / multiple) * multiple;

  int b = a + multiple;

  return (n - a >= b - n) ? b : a;
}

Rectangle calc_settings_rec(float screen_width, float screen_height) {
  if (screen_width > 1000) {
    return (Rectangle){
        .x      = (screen_width - 550) / 2,
        .y      = 50,
        .width  = 500,
        .height = screen_height * (3.0f / 4.0f),
    };
  } else if (screen_width > 800) {
    return (Rectangle){
        .x      = (screen_width - 550) / 2,
        .y      = 50,
        .width  = 550,
        .height = screen_height * (3.0F / 4.0F),
    };
  } else if (screen_width > 600) {
    return (Rectangle){
        .x      = (screen_width - 450) / 2,
        .y      = 50,
        .width  = 450,
        .height = screen_height * (3.0F / 4.0F),
    };
  } else if (screen_width > 400) {
    return (Rectangle){
        .x      = (screen_width - 350) / 2,
        .y      = 50,
        .width  = 350,
        .height = screen_height * (3.0F / 4.0F),
    };
  } else {
    return (Rectangle){
        .x      = (screen_width - 320) / 2,
        .y      = 50,
        .width  = 320,
        .height = screen_height * (3.0F / 4.0F),
    };
  }
}

void state_show_scene_settings(state* const s) {
  Rectangle menu_rec =
      calc_settings_rec(GetCorrectedScreenWidth(), GetCorrectedScreenHeight());

  DrawRectangleRec(menu_rec, WHITE);

  DrawTextEx(s->font, s->scene.scene_howto,
             (Vector2){menu_rec.x + 10, menu_rec.y + 10}, 16, 0, BLACK);
}

void init_simul_2d_keybinds(state* const s) {
  if (IsKeyPressed(KEY_S)) {
    toggle_show_settings(s);
  }

  grid* g = s->scene.scene_data.simul_2d.g;
  if (IsKeyDown(KEY_UP) && !s->show_settings) {
    if (shift_pressed()) {
      g->basis.j.y += g->step;
    } else {
      g->basis.i.y += g->step;
    }
  }

  if (IsKeyDown(KEY_DOWN) && !s->show_settings) {
    if (shift_pressed()) {
      g->basis.j.y -= g->step;
    } else {
      g->basis.i.y -= g->step;
    }
  }

  if (IsKeyDown(KEY_LEFT) && !s->show_settings) {
    if (shift_pressed()) {
      g->basis.j.x -= g->step;
    } else {
      g->basis.i.x -= g->step;
    }
  }

  if (IsKeyDown(KEY_RIGHT) && !s->show_settings) {
    if (shift_pressed()) {
      g->basis.j.x += g->step;
    } else {
      g->basis.i.x += g->step;
    }
  }

  if (IsKeyPressed(KEY_SPACE) && !s->show_settings) {
    g->basis.i = (Vector2){1, 0};
    g->basis.j = (Vector2){0, 1};
  }

  if (shift_pressed() && IsKeyPressed(KEY_EQUAL) && !s->show_settings) {
    g->line_count += 2;
  }

  if (IsKeyPressed(KEY_MINUS) && !s->show_settings) {
    g->line_count -= 2;
  }

  if (IsKeyPressed(KEY_U)) {
    s->scene.scene_data.simul_2d.show_hud =
        !s->scene.scene_data.simul_2d.show_hud;
  }

  if (IsKeyPressed(KEY_T)) {
    state_toggle_theme(s);
  }

  if (IsKeyPressed(KEY_P)) {
    grid* g      = s->scene.scene_data.simul_2d.g;
    float radius = 5;

    Vector2 pos = GetCorrectedMousePosition();

    int screen_width  = GetCorrectedScreenWidth();
    int screen_height = GetCorrectedScreenHeight();

    Vector2 origin = g->origin;

    float dx = pos.x - origin.x;
    float dy = pos.y - origin.y;

    if (shift_pressed()) {
      if (dx > 0) {
        pos.x = round_to_multiple(fabsf(dx), 40) + origin.x;
      } else {
        pos.x = origin.x - round_to_multiple(fabsf(dx), 40);
      }

      if (dy > 0) {
        pos.y = origin.y + round_to_multiple(fabsf(dy), 40);
      } else {
        pos.y = origin.y - round_to_multiple(fabsf(dy), 40);
      }
    }

    bool collision = false;
    for (int i = 0; i < g->num_points; i++) {
      Vector2 point = g->points[i];
      if (CheckCollisionPointCircle(pos, point, 5)) {
        memcpy(&g->points[i], &g->points[i + 1],
               (g->num_points - i - 1) * sizeof(Vector2));

        g->num_points -= 1;
        collision = true;
      }
    }

    if (!collision) {
      if (g->num_points != 25) {
        g->points[g->num_points++] = pos;
      } else {
        memcpy(&g->points[0], &g->points[1], 24 * sizeof(Vector2));
        g->points[24] = pos;
      }
    }
  }

  if (IsKeyPressed(KEY_C)) {
    g->num_points = 0;
  }

  if (shift_pressed() && IsKeyPressed(KEY_COMMA)) {
    g->step -= 0.05;
  }
  if (shift_pressed() && IsKeyPressed(KEY_PERIOD)) {
    g->step += 0.05;
  }
}

void state_update(state* const s) {
  float screen_width  = GetCorrectedScreenWidth();
  float screen_height = GetCorrectedScreenHeight();

  grid*   g      = s->scene.scene_data.simul_2d.g;
  Vector2 origin = g->origin;

  switch (s->scene.tag) {
    case SCENE_2D_INTERACTIVE_SIMUL:

      g->origin.x = screen_width / 2;
      g->origin.y = screen_height / 2;

      for (int i = 0; i < g->num_points; i++) {
        g->points[i] = translate_xy(g->points[i], g->origin.x - origin.x,
                                    g->origin.y - origin.y);
      }

      break;
    default:
      break;
  }
}

void state_init_scene_keybinds(state* const s) {
  switch (s->scene.tag) {
    case SCENE_2D_INTERACTIVE_SIMUL:
      init_simul_2d_keybinds(s);
      break;
    default:
      break;
  }
}

void state_toggle_theme(state* const s) {
  if (s->theme == DARKMODE) {
    s->theme = LIGHTMODE;
  } else {
    s->theme = DARKMODE;
  }

  switch (s->scene.tag) {
    case SCENE_2D_INTERACTIVE_SIMUL:
      if (s->theme == LIGHTMODE) {
        s->scene.scene_data.simul_2d.g->style = BUILTIN_THEMES.light;
      } else {
        s->scene.scene_data.simul_2d.g->style = BUILTIN_THEMES.dark;
      }
      break;
    default:
      break;
  }
}
