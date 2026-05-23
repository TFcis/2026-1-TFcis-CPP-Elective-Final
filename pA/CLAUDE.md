# pA

## Problem Description
Given n rounds of guesses and responses in the form of "X A Y B", determine the following:
- Whether the responses are contradictory
- Whether the solution is unique

If there is no contradiction and the solution is unique, output that solution.
If there is a contradiction, output `Impossible`.
If the solution is not unique, output `Ambiguous`.

Time Limit is 3 seconds.

### detail rule

Compare the guess and the answer position by position.
- **A** counts positions where the guess's digit equals the answer's digit at the same position.
- **B** is computed by, for each digit 0–9, taking `min(occurrences in guess, occurrences in answer)`, summing these over all 10 digits, and then subtracting **A**.

Equivalently: each digit of the answer can be "used" at most once when matching against the guess. First, all exact-position matches are paired up and contribute to **A**. 
Then, among the remaining unpaired digits, each guess-digit is matched to at most one unpaired answer-digit of the same value (in any position); each such match contributes to **B**.

The "each digit can be used at most once" framing is what people usually find most intuitive, and it makes the `122222` vs `111222` example obvious: 
after pairing the four positional matches (positions 1, 4, 5, 6), the unmatched guess digits are `{2, 2}` and the unmatched answer digits are `{1, 1}` — no shared values, so B = 0.

## Input Format and Constraints
The first line contains a single integer n.
The following n lines each contain 3 integers: the guessed number, the number of A's, and the number of B's, separated by spaces.
- n <= 255
- The guessed number is always a 6-digit number; repeated digits are allowed.

## Subtasks
- 15%: n = 1
- 15%: The answer is guaranteed to be unique, and all 6 digits of the answer are the same.
- 15%: The answer is guaranteed to be unique, and the first 2 digits of the answer are both 0.
- 15%: The answer is guaranteed to be unique; no contradiction will occur.
- 15%: The answer may not be unique, but there will be no contradiction.
- 25%: No additional constraints.

## Algorithm of Intended Solution
Enumerate all numbers from 000000 to 999999 and check each against all n rounds of guesses. Count how many numbers are consistent with all responses.
Rough operation count: 1,000,000 × 6 × n.

## Anticipated Wrong Solutions
- [WA] The answer is stored as an integer and output directly, forgetting to pad with leading zeros.
- [WA] When padding leading zeros, the digit count of the integer 0 is computed as 0 instead of 1.
- [WA] Used `scanf()` with `%i` to read guesses (if a guess contains leading zeros, it will be interpreted as octal).
- [WA] Incorrectly assumed that a unique solution exists only when a response of 6A0B appears (counterexamples exist).
- [WA] Incorrectly concluded that the solution is unique and stopped accepting input immediately after a 6A0B response appeared (counterexample: later input can introduce contradictions).
