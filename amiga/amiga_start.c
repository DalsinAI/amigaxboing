/* The Amiga port's start: the stack, the version string and PROGDIR:User
 * (the scores, the saved game, the settings and the player's own levels),
 * made before main() so whichever file the game writes first has its
 * drawer.
 * Copyright (c) 2026 AmiCradle Team. MIT, like XBoing. */
#include <sys/stat.h>

/* A Shell gives 4 KB of stack; SDL 2 wants more. libnix's startup sets
 * this much up. */
unsigned long __stack = 256 * 1024;
static const char version[] __attribute__((used)) =
    "$VER: XBoing 1.0.12 (16.11.2026)";

static void __attribute__((constructor)) amiga_user_drawer(void)
{
	mkdir("PROGDIR:User", 0755);
	mkdir("PROGDIR:User/xboing", 0755);
}
