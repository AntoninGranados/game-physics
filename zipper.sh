#!/bin/sh
# usage: ./zipper.sh <exercise number>
set -e

[ -d "Scenes/exercice_$1" ] || { echo "usage: $0 <exercise number>" >&2; exit 1; }

ZIP="$(pwd)/Group8_Ex$1.zip"
STAGE=$(mktemp -d)
trap 'rm -rf "$STAGE"' EXIT

mkdir "$STAGE/Scenes"
cp Scenes/*.cpp Scenes/*.h "$STAGE/Scenes/"
cp -R "Scenes/exercice_$1" "$STAGE/Scenes/"
printf '#pragma once\n#define PREFIX(ex, scene) \"\"\n#define EXERCICE_%s\n' "$1" > "$STAGE/Scenes/Exercice.h"

rm -f "$ZIP"
cd "$STAGE" && zip -r "$ZIP" Scenes -x '*.DS_Store'
