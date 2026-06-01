# pB
## Problem Description
Given a string S of length n, and two strings A and B, where all characters in A and B are guaranteed to be pairwise distinct.

- If a character in S appears in A, delete it.
- If a character in S appears in B, say character c, replace it with `QcQ`. Newly inserted Q's are not subject to deletion or replacement.

For example, given S = `abcABCxyz`, A = `cQy`, B = `xaB`,
the answer is `QaQbAQBQCQxQz`.

For convenience of input, if A or B is exactly `(null)`, treat it as an empty string.

Find the resulting string.
Characters are in the ASCII range 33 to 126.

## Input Format and Constraints
The first line is the string S.
The second line is the string A.
The third line is the string B.
There are guaranteed to be no leading or trailing spaces on any line.
- n <= 1e6

## Subtasks
- 10%: |S| <= 100, deletion only, |A| = 1
- 10%: |S| <= 100, replacement only, |B| = 1
- 10%: Deletion only
- 10%: Replacement only
- 20%: S is formed by repeating a length-8 string cyclically
- 20%: |S| <= 2000
- 20%: No additional constraints

## Algorithm of Intended Solution
Scan through S character by character to build a new string T.
- If the character is in A (deletion), do nothing.
- If the character is in B (replacement), append the replaced string to T.
- Otherwise, append the original character to T.

Time complexity: O(n).

## Anticipated Wrong Solutions
- [TLE] Using `std::string::find` / `insert` / `erase`, leading to O(n²).
- [WA] Incorrect ordering causes the Q's introduced by replacement to be deleted.
- [TLE] Using `insert` without properly tracking indices, causing newly inserted Q's to be replaced again, resulting in an infinite loop.
- [WA/RE] Not using `insert` but failing to track the change in string length, causing out-of-bounds access (especially when the result is longer than S).
- [WA] Treating `(null)` literally as a character set for deletion or replacement, instead of as an empty string.
- [WA] Specifically handling the cyclic subtask but only checking for one repetition before concluding (e.g., `AA` is mistakenly identified as an `A` cycle, `ABCABC` as an `ABC` cycle, or `ABCABCA` as an `ABC` cycle).