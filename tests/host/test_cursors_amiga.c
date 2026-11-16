/* src/sdl2_cursor.c with amiga/patches, built as for AmigaOS against
 * sdlstub/: no system cursor is made (SDL2.module's SDL_FreeCursor never
 * returns for one), every id has a cursor, the ids without a bitmap share
 * SDL's default cursor, and destroying frees only the editor's own two.
 * Copyright (c) 2026 AmiCradle Team. MIT, like XBoing. */
#include "sdl2_cursor.h"
#include <SDL2/SDL.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

int stub_system_made, stub_custom_made, stub_freed_other, stub_freed_custom;
SDL_Cursor stub_default = {100};
static SDL_Cursor customs[8];

SDL_Cursor *SDL_CreateSystemCursor(SDL_SystemCursor id)
{
    static SDL_Cursor sys[8];
    stub_system_made++;
    sys[id].id = 200 + (int)id;
    return &sys[id];
}
SDL_Cursor *SDL_CreateCursor(const Uint8 *d, const Uint8 *m, int w, int h, int hx, int hy)
{
    (void)d; (void)m; (void)w; (void)h; (void)hx; (void)hy;
    customs[stub_custom_made].id = 300 + stub_custom_made;
    return &customs[stub_custom_made++];
}
void SDL_FreeCursor(SDL_Cursor *c)
{
    /* SDL's own SDL_FreeCursor returns at once for the default cursor */
    if (c == &stub_default)
        return;
    if (c >= customs && c < customs + 8)
        stub_freed_custom++;
    else
        stub_freed_other++;
}
SDL_Cursor *SDL_GetDefaultCursor(void) { return &stub_default; }
void SDL_SetCursor(SDL_Cursor *c) { (void)c; }
int SDL_ShowCursor(int on) { return on; }
const char *SDL_GetError(void) { return ""; }
void SDL_LogWarn(int cat, const char *fmt, ...) { (void)cat; (void)fmt; }

static int failed;
static void check(const char *what, int ok)
{
    printf("%s  %s\n", ok ? "pass" : "FAIL", what);
    if (!ok) failed = 1;
}

int main(void)
{
    sdl2_cursor_status_t st = SDL2CUR_ERR_CREATE_FAILED;
    sdl2_cursor_t *c = sdl2_cursor_create(&st);
    check("created", c != NULL && st == SDL2CUR_OK);
    check("no system cursor made", stub_system_made == 0);
    check("the editor's plus and skull are its own bitmaps", stub_custom_made == 2);
    for (int id = 0; id < SDL2CUR_COUNT; id++)
        check("every id can be set", sdl2_cursor_set(c, (sdl2_cursor_id_t)id) == SDL2CUR_OK);
    sdl2_cursor_destroy(c);
    check("destroy frees the two bitmaps", stub_freed_custom == 2);
    check("and nothing else", stub_freed_other == 0);
    return failed;
}
