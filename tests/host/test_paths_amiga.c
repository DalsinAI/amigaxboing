/* Upstream's paths.c with amiga/patches applied, built on the host with
 * __amigaos__ set: every file the game reads must be in PROGDIR:Data and
 * every file it writes in PROGDIR:User, and nothing may name $HOME, an
 * XDG folder or a Unix path (a leading / is the parent drawer on AmigaOS).
 * MIT, Copyright (c) 2026 AmiCradle Team. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "paths.h"
#include "xboing_paths.h"

static int fails;

static void expect_prefix(const char *what, paths_status_t st, const char *got, const char *prefix)
{
    if (st != PATHS_OK || strncmp(got, prefix, strlen(prefix)) != 0 || strstr(got, ":/") ||
        strstr(got, "//"))
    {
        printf("FAIL %s: status %d, \"%s\" (wanted %s...)\n", what, (int)st, got, prefix);
        fails++;
    }
    else
        printf("ok   %s: %s\n", what, got);
}

int main(void)
{
    paths_config_t cfg;
    char buf[PATHS_MAX_PATH];

    /* a Linux shell's environment must change nothing */
    setenv("HOME", "/home/someone", 1);
    setenv("XDG_DATA_HOME", "/home/someone/.local/share", 1);
    setenv("XDG_DATA_DIRS", "/usr/local/share:/usr/share", 1);
    setenv("XBOING_SCORE_FILE", "/var/games/xboing/scores.dat", 1);

    if (paths_init(&cfg) != PATHS_OK)
    {
        printf("FAIL paths_init\n");
        return 1;
    }
    if (cfg.xdg_data_dirs_count != 0 || cfg.xboing_score_file[0] != '\0')
    {
        printf("FAIL XDG data dirs or a shared score file were taken\n");
        fails++;
    }
    expect_prefix("XBOING_INSTALLED_IMAGES_DIR", PATHS_OK, XBOING_INSTALLED_IMAGES_DIR, "PROGDIR:Data/");
    expect_prefix("XBOING_INSTALLED_SOUNDS_DIR", PATHS_OK, XBOING_INSTALLED_SOUNDS_DIR, "PROGDIR:Data/");
    expect_prefix("XBOING_INSTALLED_FONTS_DIR", PATHS_OK, XBOING_INSTALLED_FONTS_DIR, "PROGDIR:Data/");
    expect_prefix("levels (read)", paths_levels_dir_readable(&cfg, buf, sizeof buf), buf, "PROGDIR:Data/");
    expect_prefix("sounds (read)", paths_sounds_dir_readable(&cfg, buf, sizeof buf), buf, "PROGDIR:Data/");
    expect_prefix("levels (write)", paths_levels_dir_writable(&cfg, buf, sizeof buf), buf, "PROGDIR:User/");
    expect_prefix("personal scores", paths_score_file_personal(&cfg, buf, sizeof buf), buf, "PROGDIR:User/");
    expect_prefix("saved game", paths_save_info(&cfg, buf, sizeof buf), buf, "PROGDIR:User/");
    expect_prefix("saved level", paths_save_level(&cfg, buf, sizeof buf), buf, "PROGDIR:User/");
    expect_prefix("user data", paths_user_data_dir(&cfg, buf, sizeof buf), buf, "PROGDIR:User");
    expect_prefix("config home", PATHS_OK, cfg.xdg_config_home, "PROGDIR:User");

    printf("%s: %d failed\n", fails ? "FAIL" : "PASS", fails);
    return fails != 0;
}
