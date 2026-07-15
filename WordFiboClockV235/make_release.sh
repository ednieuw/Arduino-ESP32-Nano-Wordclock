#!/usr/bin/env bash
#
# make_release.sh - generate a single-variant RELEASE sketch from the unified
#                   WordFiboClockV230 dev sketch.
#
# Non-destructive: only writes to the output folder; the dev sketch is untouched.
# It (1) copies the .ino + shared headers + the chosen variant's headers,
#    (2) renames the .ino to match the new folder (Arduino requires this),
#    (3) flips the single clock-type #define block so the build is locked to
#        that one variant. The #if/#elif scaffolding stays in place (inert).
#
# Usage:
#   ./make_release.sh fibonacci                 [outname]
#   ./make_release.sh wordclock  FACE_DEFINE    [outname]
#     e.g.  ./make_release.sh wordclock NL144CLOCK
#
# Output goes to ../release/<outname> by default; override with RELEASE_DIR=...
#
set -euo pipefail

SRC_DIR="$(cd "$(dirname "$0")" && pwd)"
SRC_INO="$SRC_DIR/WordFiboClockV230.ino"

SHARED=(Webpage.h SoftAP.h OTAhtml.h LogViewer.h)          # included by every build
FACES=(NL144CLOCK UK144CLOCK FR144CLOCK DE144CLOCK NL92CLOCK FOURLANGUAGECLOCK \
       NLM1M2M3M4L94 NLM1M2M3M4L114 NLM1M2M3M4L144 NLM1M2M3M4L161 \
       NLM1M2M3M4L256 NLM1M2M3M4L294 VIERTALENKLOK)        # all word-clock face types

variant="${1:-}"
case "$variant" in
  fibonacci|FIBONACCI)
    keep=(Fibonacci.h)
    outname="${2:-FibonacciClockV230}"
    chosen_face=""                                          # fibonacci: no face
    ;;
  wordclock|WORDCLOCK)
    chosen_face="${2:-}"
    [ -n "$chosen_face" ] || { echo "wordclock needs a face #define, e.g. NL144CLOCK"; exit 1; }
    keep=(WordClock.h ClockFaces.h)
    outname="${3:-${chosen_face}_V230}"
    ;;
  *)
    echo "Usage: $0 fibonacci [outname] | wordclock FACE_DEFINE [outname]"; exit 1;;
esac

OUT="${RELEASE_DIR:-$SRC_DIR/../release}/$outname"
rm -rf "$OUT"; mkdir -p "$OUT"

# Copy the .ino renamed to match the folder, plus shared + variant headers.
cp "$SRC_INO" "$OUT/$outname.ino"
for f in "${SHARED[@]}" "${keep[@]}"; do cp "$SRC_DIR/$f" "$OUT/"; done
INO="$OUT/$outname.ino"

# --- helpers: toggle a top-level "#define NAME" line, preserving any trailing
#     comment. Only "#define" lines are touched (never "#if defined(NAME)"). ---
comment()   { sed -i '' -E "s|^([[:space:]]*)#define([[:space:]]+$1([[:space:]].*)?)$|\1//#define\2|"      "$INO"; }
uncomment() { sed -i '' -E "s|^([[:space:]]*)//+[[:space:]]*#define([[:space:]]+$1([[:space:]].*)?)$|\1#define\2|" "$INO"; }

if [ -n "$chosen_face" ]; then                              # WORD CLOCK release
  comment FIBONACCI
  for f in "${FACES[@]}"; do comment "$f"; done
  uncomment "$chosen_face"
else                                                        # FIBONACCI release
  for f in "${FACES[@]}"; do comment "$f"; done
  uncomment FIBONACCI
fi

echo "Release written to: $OUT"
echo "Files:"; ls -1 "$OUT"
echo "Active #define lines:"; grep -nE "^[[:space:]]*#define[[:space:]]+(FIBONACCI|$(IFS='|'; echo "${FACES[*]}"))" "$INO" || true
