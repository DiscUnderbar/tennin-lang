#!/usr/bin/env bash
# build.sh — compile a .tenn file to a native executable and (optionally) run it.
# usage: ./build.sh examples/hello.tenn [out]
set -euo pipefail

SRC="${1:?usage: build.sh <file.tenn> [out]}"
OUT="${2:-a.out}"
DIR="$(cd "$(dirname "$0")" && pwd)"
WORK="$(mktemp -d)"

# 1. build the compiler (once per run; cheap)
g++ -std=c++17 -O2 -o "$WORK/CTennin" "$DIR/CTennin.cpp"

# 2. Tennin -> LLVM IR
"$WORK/CTennin" "$SRC" > "$WORK/out.ll"

# 3. LLVM IR -> object (llvmlite) -> native link (g++)
python3 "$DIR/lower.py" "$WORK/out.ll" "$WORK/out.o"
g++ "$WORK/out.o" -o "$OUT"

echo "built: $OUT"
echo "---- generated IR ----"
cat "$WORK/out.ll"
