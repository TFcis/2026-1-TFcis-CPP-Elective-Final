# TPS Project Structure

## Per-problem layout (`pX/`)
- `problem.json` — task config (title, type, time/memory limit, score mode).
- `subtasks.json` — subtask definitions, scoring, and validator wiring.
- `solutions.json` — `{ "file.cpp": { "verdict": "model_solution"|"correct"|"time_limit"|"incorrect"|... } }`. Exactly one `model_solution` required. Optional `except: { subtask_name: alt_verdict }` for partial-credit solutions.
- `statement/index.md` — problem statement. **Do not edit.**
- `gen/`
  - `gen.cpp` — generator (built to `gen.exe`).
  - `data` — driver file listing all tests, grouped by subtask.
  - `manual/*.in` — hand-written inputs referenced by `manual <file>`.
  - `testlib.h` — local copy; treat as immutable.
- `validator/validator.cpp` (+ `testlib.h`).
- `solution/*.cpp` — model + alternative/wrong solutions.
- `scripts/` — TPS internals. **Never modify.**
- `sandbox/logs/` — per-test artifacts after `tps gen` (`.score`, `.verdict`, `.reason`).

## `subtasks.json`
```jsonc
{
  "global_validators": ["validator.cpp"],                // run for every test
  "subtask_sensitive_validators": ["validator.cpp {subtask}"], // {subtask} substituted with subtask key
  "subtasks": {
    "samples": { "index": 0, "score": 0, "text": "範例輸入輸出", "validators": [] },
    "subtask1": { "index": 1, "score": 15, "text": "...", "validators": [] },
    "full":    { "index": N, "score": ..., "text": "無額外限制", "validators": [] }
  }
}
```
- At least one of `global_validators` / `subtask_sensitive_validators` must exist (enforced by `scripts/internal/verify.py`).
- Sum of non-sample scores must equal 100. `samples` must have score 0 and is required unless OutputOnly.
- Subtask `index` values are checked for completeness (no gaps).
- A validator entry is a command line: first token resolves to a file in `validator/`, the rest are CLI args passed to the binary (after compilation).
- Use `text` in Traditional Chinese — it's rendered in the statement subtask block.

## `gen/data` syntax (parser: `scripts/internal/gen_data_parser.py`)
- `# ...` comment.
- `@subtask <name>` / `@testset <name>` — start a new group.
- `@include <name> [<name>...]` — copy all tests from prior subtask/testset (note the `@` — `include` without `@` is invalid).
- `manual <file>` — copy `manual/<file>` as the input.
- `copy <path>` — copy any file as the input.
- `gen <args...>` — execute `./gen.exe <args...>` and capture stdout as the input.
- Test names auto-generated as `<subtaskIndex>-<seq>` (e.g. `4-12`).

## Compilation environment
- `gen/Makefile` and `validator/Makefile` both compile with `g++ -std=gnu++1y -Wall -Wextra -O2 -static`.
- **`gnu++1y` is C++14, NOT C++17.** Avoid in generator/validator: structured bindings (`auto [a,b] = ...`), `if constexpr`, `std::optional`, `std::variant`, fold expressions, inline variables. Use `std::pair::first/second`, `std::get<I>()`, `std::tie`.
- `solution/`'s compile uses `correct` C++17 (per CLAUDE.md rules) — solutions can use C++17.
- `bits/stdc++.h` is fine.

## testlib invocation contracts
- **Validator** (`registerValidation(argc, argv)`):
  - testlib consumes `--testset NAME`, `--group NAME`, `--testOverviewLogFileName FILE` and their values; everything else remains in `argv`.
  - For per-subtask args, use `subtask_sensitive_validators: ["validator.cpp {subtask}"]`. The subtask name appears as a positional arg; skip testlib's flags when scanning argv.
  - Strict reading: `inf.readInt(lo, hi, "name")`, `inf.readSpace()`, `inf.readEoln()`, `inf.readEof()`. Use `ensuref(cond, fmt, ...)` for printf-style assertions.
- **Generator** (`registerGen(argc, argv, 1)`):
  - All of `argv` feeds the random seed → vary trailing args (even unused) to get distinct tests from the same generator mode (e.g. `gen unique 0 1` vs `gen unique 0 2`).
  - Use `rnd.next(lo, hi)` for inclusive ranges. Returns `long long`; cast to `int` if needed.

## Running TPS on this machine
- `tps` is **not on PATH for native Git Bash / PowerShell**; only available in **WSL**.
- Pattern: `wsl bash -c "cd /mnt/c/Users/YUKI/Documents/2026-1-TFcis-CPP-Elective-Final/pX && tps gen 2>&1"`.
- `tps gen` does: compile generator → compile validator → compile model solution → run each `gen` line → validate input against assigned subtasks' validators → run model solution to produce `.out`.
- Output is one line per test: `<name>  gen[STATUS]  val[STATUS]  sol[STATUS]`.
- `val[FAIL]` reasons are saved in `sandbox/logs/<test>.val.*` files.

## Project-wide constraints (already in root CLAUDE.md, but worth surfacing)
- Default time limit 1s; budget ~10⁸ simple ops/sec.
- Order of work: **validator → generator → solution(s)**.
- Validator must strictly check every space/newline/EOF and per-subtask special constraints.
- Testdata must be strong enough to WA wrong solutions and TLE inefficient ones.
- Keep total tests ≤ 200 per problem.
- Don't touch `.md` files, `problem.json` keys you didn't author, TPS scripts, or testlib copies.

## Common gotchas
- Forgetting `@` before `include` → parse error.
- Writing C++17 in `gen.cpp` or `validator.cpp` → Makefile rejects.
- Two `gen` lines with identical argv produce identical inputs — always add a trailing seed-varying token.
- A validator that does heavy enumeration (e.g. iterating 10⁶ candidates per test) is acceptable for one-shot validation, but early-exit aggressively or you'll add minutes to `tps gen` over 200 tests.