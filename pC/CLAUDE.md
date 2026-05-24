# pC
## Problem Description
Given an n × m map with k slimes, each initially occupying 1 cell, with at most 1 slime per cell.
Each day, every cell occupied by a slime expands outward to claim all 8 surrounding empty cells. Obstacles cannot be claimed. If multiple slimes reach the same empty cell in the same round, a territorial dispute begins and no single slime can fully claim it. These cells are called war zones.

Find, after expansion reaches a stable state, the number of cells each slime occupies and the number of war zone cells.

Coordinates use the top-left as (0, 0) and bottom-right as (n, m). The map uses `.` for empty cells, `#` for obstacles, and `S` for initial slime positions.

The first line of output is the number of war zone cells.
The following k lines each give the number of cells occupied by each slime, ordered by the row of their starting position (smaller row first; ties broken by smaller column first).

## Input Format and Constraints
The first line contains 2 positive integers n and m separated by a space.
The following n lines each contain m characters representing the initial state of the map.
- 1 <= n, m, k <= 100

## Subtasks
- 20%: Only 1 slime
- 20%: The map has only one row (size 1 × m) and no obstacles
- 20%: No obstacles
- 20%: All slimes are completely physically isolated from each other
- 20%: No additional constraints

## Algorithm of Intended Solution
Simulate each slime's daily expansion.
Time complexity: O(n²m²).

## Anticipated Wrong Solutions
- [WA] Slimes claim obstacle cells.
- [WA] Due to the order in which cells are updated within a round, cells that change from empty to claimed in the current round are mistakenly treated as already claimed, causing them to expand again in the same round.
- [WA] A cell that should be a war zone is instead claimed by whichever slime is evaluated first in that round.
- [WA] A cell that should be a war zone is mistakenly claimed by the third slime to arrive in that round (because the cell was not properly marked and was misidentified as empty).
- [WA] Running an independent BFS per slime to compute the number of rounds to reach each cell, then marking a cell as a war zone if multiple slimes tie for the minimum round count. Counterexample:
```
#.S
...
#.S
```
The leftmost cell of the 2nd row should be empty, but both slimes reach it in 2 steps via independent BFS, so it is incorrectly marked as a war zone. This approach cannot handle cases where a path is blocked midway.
- [WA] Expanding slimes one at a time, stealing cells where another slime arrived in more rounds, or set to war if arrived in the same rounds. (Similar properties, weaknesses, and counterexamples as the independent BFS approach.)
- [WA] When two slimes are exactly adjacent at the start of a round, they are mistakenly treated as war zones (confusing "already occupying a cell at the start of the round" with "about to expand into that cell this round").
- [WA] Incorrect ordering — mistakenly sorting by column first then row instead of row first then column.
- [WA/RE] Mishandling the map dimensions as m × n instead of n × m.