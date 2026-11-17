#!/bin/sh
# Host test of the Amiga path patch: fetches upstream at the locked commit
# (or takes XB=), applies amiga/patches and runs test_paths_amiga.c.
#   sh tests/host/test_paths.sh
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
for p in "$ROOT"/amiga/patches/*.patch; do (cd "$T/xb" && patch -s -p1 < "$p"); done
${CC:-cc} -std=gnu11 -Wall -Wextra -D__amigaos__ -DXBOING_DATA_DIR='"PROGDIR:Data"' \
    -I"$T/xb/include" -o "$T/test_paths" "$HERE/test_paths_amiga.c" "$T/xb/src/paths.c"
# on the host "PROGDIR:Data/levels" is just a relative path: the drawer the
# game finds its data in, as on the Amiga
cd "$T" && mkdir -p "PROGDIR:Data/levels" "PROGDIR:Data/sounds" && ./test_paths
