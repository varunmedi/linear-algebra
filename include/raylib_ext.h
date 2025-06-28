#pragma once

#ifdef PLATFORM_WEB

#include <emscripten/emscripten.h>
#include <emscripten/html5.h>
#include <raylib.h>

Vector2 GetCorrectedMousePosition(void);
void    SetupMouseCallback();

// TODO: Figure out how to make this work so that a small widget still
// has reasonable behavior for placing points
// int GetCorrectedScreenWidth();
// int GetCorrectedScreenHeight();

#define GetCorrectedScreenWidth  GetScreenWidth
#define GetCorrectedScreenHeight GetScreenHeight

#else

#define GetCorrectedMousePosition GetMousePosition
#define GetCorrectedScreenWidth   GetScreenWidth
#define GetCorrectedScreenHeight  GetScreenHeight

#endif
