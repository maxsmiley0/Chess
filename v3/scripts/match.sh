#!/usr/bin/env bash
# Runs a fastchess match. Defaults to self-play (regression baseline).
#
#   scripts/match.sh                                   self-play, 100 games
#   scripts/match.sh -o ~/tools/Stockfish/src/stockfish -e 1400
#   scripts/match.sh -b ./smv3.old -g 400 -t 10+0.1
set -euo pipefail

cd "$(dirname "$0")/.."

TOOLS="${TOOLS:-$HOME/tools}"
FASTCHESS="$TOOLS/fastchess/fastchess"

ENGINE=./smv3
BASELINE=./smv3
OPPONENT=""
GAMES=100
TC=8+0.08
CONCURRENCY=$(sysctl -n hw.ncpu 2>/dev/null || nproc)
LIMIT_ELO=""
SPRT=""

usage() {
    grep '^#' "$0" | cut -c3-
    exit 1
}

while getopts "e:o:b:g:t:c:sh" opt; do
    case $opt in
        o) OPPONENT=$OPTARG ;;
        b) BASELINE=$OPTARG ;;
        g) GAMES=$OPTARG ;;
        t) TC=$OPTARG ;;
        c) CONCURRENCY=$OPTARG ;;
        e) LIMIT_ELO=$OPTARG ;;
        s) SPRT="-sprt elo0=0 elo1=10 alpha=0.05 beta=0.05" ;;
        h|*) usage ;;
    esac
done

[ -x "$FASTCHESS" ] || { echo "fastchess missing; run scripts/get-tools.sh"; exit 1; }
[ -x "$ENGINE" ]    || { echo "engine missing; run make"; exit 1; }

BOOK=test/openings.epd
[ -f "$BOOK" ] || ./smv3_test --openings test/openings.txt "$BOOK" >/dev/null

OPP_ARGS=(cmd="$BASELINE" name=baseline)
if [ -n "$OPPONENT" ]; then
    OPP_ARGS=(cmd="$OPPONENT" name="$(basename "$OPPONENT")")
    if [ -n "$LIMIT_ELO" ]; then
        OPP_ARGS+=(option.UCI_LimitStrength=true "option.UCI_Elo=$LIMIT_ELO")
    fi
fi

mkdir -p logs

set -x
"$FASTCHESS" \
    -engine cmd="$ENGINE" name=smv3 \
    -engine "${OPP_ARGS[@]}" \
    -each proto=uci tc="$TC" \
    -openings file="$BOOK" format=epd order=random \
    -games 2 -rounds $((GAMES / 2)) \
    -repeat \
    -concurrency "$CONCURRENCY" \
    -recover \
    -pgnout file=logs/match.pgn \
    $SPRT
