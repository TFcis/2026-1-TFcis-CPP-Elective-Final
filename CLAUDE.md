# Project Overview

This is a Competitive Programming contest, using Task Preparation System.
There are 4 individual problems, `pA`, `pB`, `pC` and `pD`.
You are setting up the problems.

When working on a specific problem, read the `CLAUDE.md` located in that problem's folder.
Read @about_tps.md first if you need to understand the structure of TPS.

## Rules

### general
- Unless a target file is explicitly specified, do not make any changes to the root directory.
- Do not edit any files with the `.md` extension.
- Do not edit problem statements.
- Do not edit files in `gen/manual/` in each problem's folder.
- Do not modify important files such as libraries built into the TPS system, to avoid disrupting the structure.
- The problem intent will be explicitly stated in the `CLAUDE.md` inside the problem folder and must be followed strictly. If anything is unclear or ambiguous, ask — do not make assumptions.
- Unless otherwise specified, the default time limit is 1 second; use 1×10⁸ operations per second as a rough estimate.
- Complete the validator first, then the testdata generator, and finally write the solution to validate.

### validator
- The validator and testdata generator should make use of functions from `testlib.h` as much as possible, rather than implementing things from scratch.
- The validator must strictly follow the format, constraints, and value ranges described in the problem statement, rigorously checking every location where a space, newline, or EOF is required. Every non-empty line should be ended with '\n'.
- The validator and testdata generator must handle each subtask's special constraints with care.
- Use the same validator to verify all subtasks. Use command-line args for recognization.
- Confirm that all testdata passes the validator. If any testdata fails, fix the generator and regenerate the testdata — do not modify the verifier.

### testdata
- Testdata must be strong enough to give wrong code WA and inefficient code TLE.
- Keep testdata within 200 cases; aim to purposefully break incorrect or inefficient code.
- Testdata need to beat each Anticipated Wrong Solutions as many subtasks as possible.
- Create human readable files to report and record result that every solution versus every subtask 
- When regenerating test data, do so on a per-subtask basis and avoid regenerating unchanged subtasks whenever possible; solutions should consult logs to avoid rerunning unchanged subtasks.
- sample I/O should be created manually, not by automatically generated.
- No need to check the reason verify FAILED before completing generator.

### solution
- Solutions should cover as many cases as possible, including but not limited to: "only fast enough under specially distributed data", "Only fast enough in most cases or under random distribution", "correct in most cases but not all", "approximation-based or randomized approaches with unstable correctness", "failure to account for 32-bit integer overflow", and "plausible-looking but incorrect approaches".
- When writing solutions, think like a professional competitive programmer. Apply I/O optimization and general constant-factor optimizations by default. For I/O optimization, only go as far as `ios::sync_with_stdio(false); cin.tie(nullptr);` — do not go to the level of fread().
- When writing the intended solution, also write an alternative solution using a different algorithm, or a brute-force solution that is less efficient but guaranteed correct, in order to verify the intended solution.
- When writing non-intended solutions, imagine yourself in a scenario where you cannot figure out the intended approach, and try to score as many points as possible using an incorrect or partial solution.
- Solutions only need to target C++; other languages are not considered.
- All solutions must conform to the C++17 standard.
- Write approaches for each solution in code, using comment.
- Write a targeted but non-general solution for each subtask (except the final "no additional constraints" subtask)
- Any unexpected runtime-related behavior or results in the solutions must be reported, but no modifications should be made on your own initiative. Report the issue and wait for my judgment. Also, this environment is different from the real judge of this contest.

### subtask
- In `subtasks.json` always add an extra subtask as front, for sample I/O with score 0. 
- In `subtasks.json` content in field `text` should be written in Traditional Chinese.

### TPS
- Always use tps commands for compiling or executing. All tps commands need to be executed in WSL.
- Everytime running `tps invoke` or `tps run` should keep the output as file to log, used to prevent running same solution to same testdata repeatedly. Maintain the partially invalidation when re-gen testdata.
