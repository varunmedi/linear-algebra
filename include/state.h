#pragma once

#include <raylib.h>

#include "grid.h"

typedef enum { SCENE_2D_INTERACTIVE_SIMUL } scene_tag;
typedef enum { DARKMODE, LIGHTMODE } theme;

typedef union {
  struct {
    grid* g;
    bool  show_hud;
  } simul_2d;
} scene_data;

typedef struct {
  scene_tag   tag;
  const char* scene_howto;
  scene_data  scene_data;
} scene;

typedef struct {
  bool  show_settings;
  scene scene;
  Font  font;
  theme theme;
} state;

void run_state(state* state);
void toggle_show_settings(state* const s);
void state_show_scene_settings(state* const s);
void state_toggle_theme(state* const s);

Rectangle calc_settings_rec(float screen_width, float screen_height);

void init_simul_2d_keybinds(state* const s);
void state_init_scene_keybinds(state* const s);
void state_update(state* const s);
