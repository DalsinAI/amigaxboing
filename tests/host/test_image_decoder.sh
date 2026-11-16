#!/bin/sh
# Host test of the image decoder patch: upstream at the locked commit (or
# XB=) with amiga/patches applied must ask SDL2_image.module for its own
# 68k decoder (hint SDL_IMAGE_DECODER "cpu") on AmigaOS only, before
# SDL_Init and before the textures are read, and leave other systems alone.
#   sh tests/host/test_image_decoder.sh
# MIT, Copyright (c) 2026 AmiCradle Team.
set -eu
HERE=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
ROOT=$(CDPATH= cd -- "$HERE/../.." && pwd)
. "$ROOT/package/xboing.lock"
T=$(mktemp -d); trap 'rm -rf "$T"' EXIT
if [ -n "${XB:-}" ]; then
    mkdir -p "$T/xb"; cp -R "$XB/src" "$XB/include" "$T/xb/"
else
    git -C "$T" init -q xb
    git -C "$T/xb" fetch -q --depth 1 "$XBOING_URL" "$XBOING_COMMIT"
    git -C "$T/xb" checkout -q FETCH_HEAD
fi
for p in "$ROOT"/amiga/patches/[0-8]*.patch; do (cd "$T/xb" && patch -s -p1 < "$p"); done
F="$T/xb/src/game_init.c"
fail=0
check() { if [ "$2" = 1 ]; then echo "pass  $1"; else echo "FAIL  $1"; fail=1; fi; }
hint=$(grep -n 'SDL_SetHint("SDL_IMAGE_DECODER", "cpu");' "$F" | cut -d: -f1)
init=$(grep -n 'SDL_Init(SDL_INIT_VIDEO' "$F" | head -n 1 | cut -d: -f1)
tex=$(grep -n 'sdl2_texture_create(' "$F" | head -n 1 | cut -d: -f1)
check "the hint is set once" "$([ "$(echo "$hint" | wc -w)" = 1 ] && echo 1 || echo 0)"
check "before SDL_Init" "$([ -n "$hint" ] && [ "$hint" -lt "$init" ] && echo 1 || echo 0)"
check "before the textures are read" "$([ -n "$hint" ] && [ "$hint" -lt "$tex" ] && echo 1 || echo 0)"
# the line before the hint's comment block opens #ifdef __amigaos__ and the
# next #endif closes it after the hint
open=$(awk -v h="$hint" 'NR<h && /^#/ {l=$0} END {print l}' "$F")
close=$(awk -v h="$hint" 'NR>h && /^#/ {print; exit}' "$F")
check "only on AmigaOS" "$([ "$open" = "#ifdef __amigaos__" ] && [ "$close" = "#endif" ] && echo 1 || echo 0)"
# the patched file still preprocesses as upstream on another system
if ${CC:-cc} -E -P -I"$T/xb/include" -I"$T/xb/src" $(sdl2-config --cflags 2>/dev/null) "$F" >"$T/host.i" 2>/dev/null; then
    check "no hint on other systems" "$(grep -c SDL_IMAGE_DECODER "$T/host.i" | grep -qx 0 && echo 1 || echo 0)"
else
    echo "skip  no hint on other systems (no SDL 2 headers here)"
fi
exit $fail
