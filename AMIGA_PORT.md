XBoing for GFOS
===============

A port of XBoing, the breakout arcade game Justin C. Kibell wrote for X11
(XBoing 2.4, 1996), in J Freeman's SDL 2 version from
https://github.com/jmf-pobox/xboing-c at commit
34e6ea2868d0fd0d20784da281f176530a7e1bae (version 1.0.12), for GFOS:
AmigaOS 3.2 with GFOpenUp, on a 68040 or 68060 with an FPU, on GFOpenUp's
SDL 2.

Bounce the proton ball off your paddle and clear every block, across 80
levels of bombs, teleports, sticky paddles, extra balls and bullets. A
level editor is built in.

Status: built in Cradle's os32-gcc16-open6 stove; not yet run in a lab.


What you need
-------------

- GFOS: AmigaOS 3.2 with GFOpenUp 0.8.26 or later, which installs
  LIBS:GFOpenGPU/SDL2.module, SDL2_image.module and SDL2_mixer.module.
- A 68040 or 68060 with an FPU (not an LC040 or EC040), an RTG screen
  with room for a 575 x 720 window (or the AmiCradle lab), and about 8 MB
  of free memory.
- Nothing else: the pictures, sounds, fonts and levels are in the drawer.


How to play
-----------

Install it one of three ways:

- GFOpenUp: `GFOpenUpTool INSTALL Parts/XBoing` puts it in SYS:Games/XBoing.
- Run Install-XBoing from the archive (it offers Work:Games).
- Or copy the XBoing drawer where you want it.

Then double-click XBoing, or from a Shell:

    cd XBoing
    XBoing [-keys] [-nosound] [-speed 1-9] [-startlevel N] [-grab]

The mouse moves the paddle and any button launches the ball and fires;
G switches to the keys. The keys are in the drawer's Controls file.

The high scores, the saved game, the settings and your own levels are
kept in the drawer's User drawer; Install-XBoing and GFOpenUp keep it
when they update the game.


Building
--------

    sh package/build-amiga.sh        (STOVE=/path/to/os32-gcc16-open6)

or, in Cradle, stove_build with the os32-gcc16-open6 stove and target
package. It fetches upstream at the commit package/xboing.lock names,
applies amiga/patches, builds with amiga/Makefile.amigaos and packs
XBoing-VERSION-68040.lha and the GFOpenUp part Parts/XBoing.

Host tests: `sh tests/host/test_paths.sh` builds upstream's paths.c with
the Amiga patch on the host and checks every path it gives is in the
drawer; `sh tests/host/test_image_decoder.sh` checks the image decoder
hint is set on AmigaOS only, before SDL_Init and the pictures.


What was changed
----------------

- amiga/patches/0001-paths-amigaos.patch (src/paths.c): the data are in
  PROGDIR:Data, the player's files in PROGDIR:User; no $HOME, XDG tree or
  shared score board.
- amiga/patches/0002-image-decoder-amigaos.patch (src/game_init.c): the
  pictures are decoded by SDL2_image's own 68k decoder (hint
  SDL_IMAGE_DECODER cpu). XBoing reads about 280 small PNGs at start-up;
  sent one by one to the services card's cores they took minutes on a
  busy Cradle, decoded here they take seconds. SetEnv SDL_IMAGE_DECODER
  auto gives the cores back.
- amiga/amiga_compat.c: getpwuid, setegid, issetugid and flock, which
  libnix doesn't have, each doing what it does for an ordinary user.
- amiga/amiga_start.c: 256 KB of stack, the $VER string, PROGDIR:User
  made at start.
- amiga/: the Makefile, the GlowIcons (our own art), the Installer script
  and its spec. package/: the build, the release archive and the GFOpenUp
  part.

The AmiCradle Team.
