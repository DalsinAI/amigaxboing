/* Host test stand-in for SDL2/SDL.h: only what src/sdl2_cursor.c asks of
 * it, counting the cursors made and freed (test_cursors_amiga.c).
 * Copyright (c) 2026 AmiCradle Team. MIT, like XBoing. */
#ifndef SDLSTUB_SDL_H
#define SDLSTUB_SDL_H
#include <stdint.h>
typedef uint8_t Uint8;
typedef struct SDL_Cursor { int id; } SDL_Cursor;
typedef enum {
    SDL_SYSTEM_CURSOR_ARROW, SDL_SYSTEM_CURSOR_WAIT, SDL_SYSTEM_CURSOR_CROSSHAIR,
    SDL_SYSTEM_CURSOR_HAND
} SDL_SystemCursor;
#define SDL_ENABLE 1
#define SDL_DISABLE 0
#define SDL_LOG_CATEGORY_APPLICATION 0
extern int stub_system_made, stub_custom_made, stub_freed_other;
extern SDL_Cursor stub_default;
SDL_Cursor *SDL_CreateSystemCursor(SDL_SystemCursor id);
SDL_Cursor *SDL_CreateCursor(const Uint8 *data, const Uint8 *mask, int w, int h, int hx, int hy);
void SDL_FreeCursor(SDL_Cursor *c);
SDL_Cursor *SDL_GetDefaultCursor(void);
void SDL_SetCursor(SDL_Cursor *c);
int SDL_ShowCursor(int on);
const char *SDL_GetError(void);
void SDL_LogWarn(int cat, const char *fmt, ...);
#endif
