/* What XBoing asks of a Unix and libnix doesn't have. AmigaOS has one
 * user, no set-id programs and no shared score board, so each call does
 * what it would for an ordinary user on Linux.
 * Copyright (c) 2026 AmiCradle Team. MIT, like XBoing. */
#include <stddef.h>
#include <sys/types.h>

struct passwd;

/* game_modes.c asks for the player's real name: there is none, so it goes
 * on to $USER and then to its own default. */
struct passwd *getpwuid(uid_t uid)
{
	(void)uid;
	return NULL;
}

/* sys_priv.c drops a set-group-id it never has here. */
int setegid(gid_t gid)
{
	(void)gid;
	return 0;
}

int issetugid(void)
{
	return 0;
}

/* Only the shared score board takes this lock, and paths.c never names
 * one on AmigaOS; one program at a time writes the drawer's own files. */
int flock(int fd, int op)
{
	(void)fd;
	(void)op;
	return 0;
}

/* Lab only: one line per step, appended to PROGDIR:User/trace.txt and
   closed each time so it can be read while the game runs. */
#include <stdio.h>
#include <time.h>
void xb_trace(const char *what)
{
    FILE *f = fopen("PROGDIR:User/trace.txt", "a");
    if (f != NULL)
    {
        fprintf(f, "%lu %s\n", (unsigned long)clock(), what);
        fclose(f);
    }
}
