#!/usr/bin/env bash
# Sweeps the engine against Stockfish capped at a series of UCI_Elo values to
# bracket its playing strength.
#
#   scripts/ladder.sh                 default rungs
#   scripts/ladder.sh 2000 2200 2400
#   GAMES=100 TC=10+0.1 scripts/ladder.sh
set -euo pipefail

cd "$(dirname "$0")/.."

TOOLS="${TOOLS:-$HOME/tools}"
FASTCHESS="$TOOLS/fastchess/fastchess"
STOCKFISH="${STOCKFISH:-$TOOLS/Stockfish/src/stockfish}"

GAMES="${GAMES:-40}"
TC="${TC:-8+0.08}"
CONCURRENCY="${CONCURRENCY:-$(sysctl -n hw.ncpu 2>/dev/null || nproc)}"

[ -x "$FASTCHESS" ] || { echo "fastchess missing; run scripts/get-tools.sh"; exit 1; }
[ -x "$STOCKFISH" ] || { echo "stockfish missing; run scripts/get-tools.sh"; exit 1; }
[ -x ./smv3 ]       || { echo "engine missing; run make"; exit 1; }

BOOK=test/openings.epd
[ -f "$BOOK" ] || ./smv3_test --openings test/openings.txt "$BOOK" >/dev/null

RUNGS=("$@")
[ ${#RUNGS[@]} -eq 0 ] && RUNGS=(1600 2000 2400 2800)

printf 'engine    ./smv3\n'
printf 'opponent  %s\n' "$STOCKFISH"
printf 'tc        %s   games %s per rung   concurrency %s\n\n' "$TC" "$GAMES" "$CONCURRENCY"
printf '%-10s %-28s %s\n' "SF_ELO" "elo diff (95%% CI)" "score"

for elo in "${RUNGS[@]}"; do
    out=$("$FASTCHESS" \
        -engine cmd=./smv3 name=smv3 \
        -engine cmd="$STOCKFISH" name="SF-$elo" \
            option.UCI_LimitStrength=true "option.UCI_Elo=$elo" \
            option.Threads=1 option.Hash=16 \
        -each proto=uci tc="$TC" \
        -openings file="$BOOK" format=epd order=random \
        -games 2 -rounds $((GAMES / 2)) -repeat \
        -concurrency "$CONCURRENCY" -recover 2>&1)

    diff=$(printf '%s' "$out" | grep -E '^Elo:' | tail -1 | sed 's/^Elo: //; s/, nElo.*//')
    score=$(printf '%s' "$out" | grep -E '^Games:' | tail -1)
    printf '%-10s %-28s %s\n' "$elo" "${diff:-n/a}" "${score:-n/a}"
done
