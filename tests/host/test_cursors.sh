#!/bin/sh
# Host test of the cursor patch: upstream at the locked commit (or XB=)
# with amiga/patches applied, src/sdl2_cursor.c built as for AmigaOS
# against tests/host/sdlstub and checked by test_cursors_amiga.c.
#   sh tests/host/test_cursors.sh
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
${CC:-cc} -std=gnu11 -Wall -D__amigaos__ -I"$HERE/sdlstub" -I"$T/xb/include" -I"$T/xb/src" \
    -o "$T/test_cursors" "$HERE/test_cursors_amiga.c" "$T/xb/src/sdl2_cursor.c"
"$T/test_cursors"
