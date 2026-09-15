#!/usr/bin/env bash
# Fetches and builds external benchmarking tools into $TOOLS (default ~/tools).
# Nothing is installed inside this repository.
set -euo pipefail

TOOLS="${TOOLS:-$HOME/tools}"
mkdir -p "$TOOLS"

echo "tools directory: $TOOLS"

if [ ! -x "$TOOLS/fastchess/fastchess" ]; then
    echo "building fastchess"
    if [ -d "$TOOLS/fastchess/.git" ]; then
        git -C "$TOOLS/fastchess" pull --ff-only
    else
        git clone --depth 1 https://github.com/Disservin/fastchess.git "$TOOLS/fastchess"
    fi
    make -C "$TOOLS/fastchess" -j"$(sysctl -n hw.ncpu 2>/dev/null || nproc)"
fi
echo "fastchess: $("$TOOLS/fastchess/fastchess" --version | head -1)"

if [ ! -x "$TOOLS/Stockfish/src/stockfish" ]; then
    echo "building stockfish (reference opponent for strength calibration)"
    if [ -d "$TOOLS/Stockfish/.git" ]; then
        git -C "$TOOLS/Stockfish" pull --ff-only
    else
        git clone --depth 1 https://github.com/official-stockfish/Stockfish.git "$TOOLS/Stockfish"
    fi

    case "$(uname -s)-$(uname -m)" in
        Darwin-arm64) ARCH=apple-silicon ;;
        Darwin-x86_64) ARCH=x86-64-avx2 ;;
        Linux-aarch64) ARCH=armv8 ;;
        *) ARCH=x86-64-avx2 ;;
    esac

    make -C "$TOOLS/Stockfish/src" -j"$(sysctl -n hw.ncpu 2>/dev/null || nproc)" \
        build ARCH="$ARCH"
fi
printf 'stockfish: '
printf 'uci\nquit\n' | "$TOOLS/Stockfish/src/stockfish" 2>/dev/null | grep '^id name' | head -1
