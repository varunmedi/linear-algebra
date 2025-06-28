#include "raylib_ext.h"

#ifdef PLATFORM_WEB

#include <emscripten/emscripten.h>
#include <emscripten/html5.h>
#include <stdio.h>

EM_JS(int, get_window_inner_width, (), { return window.innerWidth; });

EM_JS(int, get_window_inner_height, (), { return window.innerHeight; });

EM_JS(double, get_canvas_offset_left, (), {
  return document.getElementById('canvas').getBoundingClientRect().left;
});

EM_JS(double, get_canvas_offset_top, (), {
  return document.getElementById('canvas').getBoundingClientRect().top;
});

// clang-format off

EM_JS(int, GetCorrectedScreenWidth, (), {
  return document.getElementById('canvas').width;
});

EM_JS(int, GetCorrectedScreenHeight, (), {
  return document.getElementById('canvas').height;
});

// clang-format on

static Vector2 g_mouse_pos = {0, 0};
Vector2        GetEmscriptenMousePosition(void) { return g_mouse_pos; }

Vector2 GetCorrectedMousePosition(void) {
  EmscriptenMouseEvent mouse;
  emscripten_get_mouse_status(&mouse);
  Vector2 pos = GetEmscriptenMousePosition();

  return pos;
}

EM_BOOL mouse_callback(int eventType, const EmscriptenMouseEvent* e,
                       void* userData) {
  g_mouse_pos.x = (float)e->targetX;
  g_mouse_pos.y = (float)e->targetY;
  return EM_TRUE;
}

void SetupMouseCallback() {
  emscripten_set_mousemove_callback("#canvas", NULL, EM_TRUE, mouse_callback);
}

#endif
