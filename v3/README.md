# StrawberryMangoes v3

UCI chess engine. Bitboard board representation with magic-number sliding attacks,
pin-aware legal move generation, and a negamax search with quiescence.

## Build

```sh
make            # development build
make release    # optimised build, no assertions
```

Produces `smv3`, a self-contained binary with no runtime dependencies.

## Verify

```sh
make test       # perft suite, movegen vs brute-force reference, zobrist round-trip
make openings   # replay opening lines, validating every intermediate position
```

`make test` checks perft node counts against published values and, at every
position, cross-checks the fast pin-aware generator against an independent
brute-force generator and verifies the incrementally-updated Zobrist key against
a from-scratch recomputation.

## Strength

Measured with fastchess against Stockfish limited by `UCI_LimitStrength` /
`UCI_Elo`, 60 games per rung at 8+0.08, from `scripts/ladder.sh`:

| Stockfish `UCI_Elo` | Elo diff (95% CI) | Result |
| --- | --- | --- |
| 2000 | +58.45 +/- 90.00 | 35W 25L 0D (58.33%) |
| 2200 | -5.79 +/- 79.14 | 28W 29L 3D (49.17%) |
| 2400 | -107.54 +/- 97.49 | 19W 37L 4D (35.00%) |
| 2600 | -183.21 +/- 96.53 | 12W 41L 7D (25.83%) |

The crossover sits at 2200, so v3 plays at roughly 2200 on this scale at this
time control.

Two earlier runs, 40 games each at 4+0.04:

| Opponent | Result |
| --- | --- |
| Stockfish `UCI_Elo` 1320 | 40W 0L 0D (Elo diff unbounded) |
| v3 self-play | -17.39 +/- 64.12, 12W 14L 14D |

Caveats: Stockfish's `UCI_Elo` scale is not comparable to CCRL or other
published rating lists. Every figure above comes from one machine at one time
control, and the confidence intervals are wide at 60 games.

## Playing in a GUI

The engine speaks UCI and can be added to any UCI-compatible GUI.

En Croissant:

1. Download and install [En Croissant](https://encroissant.org)
2. Open the **Engines** tab
3. **Add Engine** -> **Local**
4. Point it at the absolute path of `smv3`
5. On first launch macOS may block an unsigned binary. Clear the quarantine flag:
   ```sh
   xattr -d com.apple.quarantine /path/to/smv3
   ```

### Supported UCI surface

| Command | Notes |
| --- | --- |
| `uci`, `isready`, `ucinewgame`, `quit` | |
| `position startpos \| fen ... [moves ...]` | |
| `go depth \| movetime \| nodes \| wtime/btime/winc/binc \| infinite` | all modes En Croissant issues |
| `stop` | search runs on its own thread and remains interruptible |
| `setoption name MultiPV value N` | |
| `d` | non-standard: prints board, FEN, key, legal move count |
| `legalmoves` | non-standard: prints all legal moves |

MultiPV is required by En Croissant rather than optional. It computes
`real_multipv = min(MultiPV, legal_moves)` and withholds an analysis update
until it has received sequential `info multipv 1..N` lines, so an engine that
reports only one line stalls its analysis panel. Game review forces `MultiPV=2`.

## Open issues

### MultiPV disables root alpha-beta pruning

With `MultiPV > 1` the root searches every move with a full window in order to
produce an exact score for each reported line. This is correct but slower than
`MultiPV = 1`, which retains normal root alpha-beta. Analysis at high MultiPV
values reaches lower depths than it otherwise would.

### Move ordering is captures-only

Ordering is MVV-LVA for captures, then promotions, then quiets in generation
order. There is deliberately no PV-move, transposition-table, killer, or history
ordering yet, so each can be added and measured independently.

### No transposition table

Search re-visits transposed positions. Node counts per depth are therefore far
higher than a pruned search would need.

### Evaluation is hand-crafted

Material plus tapered piece-square tables. NNUE is not wired in.

## Layout

```
src/        engine sources
test/       perft, movegen and zobrist verification, opening lines
scripts/    benchmarking helpers
```

Third-party engines and tools are referenced by path and are never vendored into
this repository.
