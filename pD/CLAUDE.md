# pD

## Problem Description
Given a string S of length n and a string T of length m, find the shortest interval (l, r) (inclusive) in S such that T is a subset of that interval. 
Output the length of the shortest such interval. All test cases are guaranteed to have a solution.

A subset is defined as: there exists at least one permutation of S[l..r] such that T is a subsequence of that permutation.

For example, when S[l..r] = `ABCA`:
- `ABC` is a subset
- `BAA` is a subset
- `CABA` is a subset
- `CAC` is not a subset

## Input Format and Constraints
The first line contains 2 positive integers n and m separated by a space.
The second line is the string S.
The third line is the string T.
- 1 <= n <= 1e6
- 1 <= m <= 1e5
- S and T consist of characters in the ASCII range 33–126.

## Subtasks
- 20%: T contains only 1 distinct character, and that character appears in S exactly as many times as the length of T.
- 20%: |T| = 2
- 20%: Both S and T are increasing sequences.
- 20%: |S| <= 2000
- 20%: No additional constraints.

## Algorithm of Intended Solution

### Approach 1
Two-pointer: enumerate the left endpoint l and move r rightward, maintaining a bucket counting the occurrences of each character, until all character counts required by T are satisfied — this gives the shortest interval with left endpoint l. 
If r reaches the end of S without satisfying the condition, then l has no valid interval. When advancing from l to l+1, r is not reset; it stays at its current position and continues moving right.

Time complexity: O(n).

### Approach 2
Maintain one array per character recording all positions where it appears in S; that is, v[i][j] = k means the j-th occurrence of character i in S is at position S[k].
If character i appears a times in T, then the interval (l, r) must span at least a elements in v[i], i.e., l <= v[i][j] and v[i][j+a-1] <= r for the count to be satisfied.
Maintain a pointer per character and advance the leftmost pointer by one step at a time, recording the interval size at each step; the minimum is the answer.

Concretely: set l = min(v[i][0]) and r = max(v[i][a-1]) as the first satisfying pair, compute interval length r-l+1. For the character where v[i][0] == l, advance to v[i][1]..v[i][a], update l and r, and recompute r-l+1. 
Repeat until some character can no longer satisfy the condition. The minimum interval length seen throughout the process is the answer.

Time complexity: O(n).

## Anticipated Wrong Solutions
- [AC] Using Approach 1 but re-checking the counts of all characters (33~126) each time r advances (a suboptimal approach, but still capable of scoring 100 points on this problem).
- [TLE] Using Approach 1 but checking the subset condition in O(m) each time r advances.
- [TLE] Binary searching for r for every l, with O(m) per subset check.
- [TLE] Using Approach 1 but resetting r to l+m-1 each time, degrading to O(n²).
- [TLE] Using Approach 1, resetting r to l+m-1 each time, and breaking early to try the next l once the interval length equals the current best. Still O(n²).
- [TLE] Using Approach 1, but pre-filtering S to build S' by removing all characters not in T, storing original indices to compute correct positions and interval lengths. Resets r to l+m-1 each time and breaks early when the interval length equals the current best. Still O(n²).
- [WA] Directly outputting m as the answer (to verify that every subtask scores 0).
- [WA] Directly outputting n as the answer (to verify that every subtask scores 0).
- [WA] Treating "subset" as "subsequence" and solving accordingly.
- [WA] Using the Approach 2 data structure v, finding the shortest satisfying interval per character, then taking the longest of those shortest intervals across all characters as the answer.
- [WA] Each round, randomly select an interval of length >= m and shorter than the current best, linearly check whether it satisfies the condition, and keep track of the shortest satisfying length. Output n if no satisfying interval is found. The number of rounds is estimated based on n and m to maximize attempts without TLE.
