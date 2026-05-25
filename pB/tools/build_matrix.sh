#!/bin/bash
# Read /tmp/pB-invoke.log; emit a markdown table of per-subtask scores.
# Score per subtask = subtask_total if every test AC (GroupMin), else 0.
LOG=${1:-/tmp/pB-invoke.log}
OUT=${2:-tools/results.md}
cd "$(dirname "$0")/.."
SOLS=(correct.cpp alt.cpp partial_sub1.cpp partial_sub2.cpp partial_del_only.cpp partial_rep_only.cpp partial_cyclic.cpp tle_quadratic.cpp wa_replace_then_delete.cpp wa_null_literal.cpp wa_cycle_one_rep.cpp wa_null_s.cpp)
SCORES=(0 10 10 10 10 20 20 20)
HEADERS=("samples(0)" "sub1(10)" "sub2(10)" "sub3(10)" "sub4(10)" "sub5(20)" "sub6(20)" "full(20)")

{
	echo "# pB solution × subtask result matrix"
	echo
	echo "Generated from \`$LOG\`. Score per subtask is awarded only if every test in that"
	echo "subtask passes (GroupMin). Cell shows \`pass/total (=score)\`."
	echo
	printf "| Solution |"
	for h in "${HEADERS[@]}"; do printf " %s |" "$h"; done
	printf " Total |\n"
	printf "|---|---|---|---|---|---|---|---|---|---|\n"

	for s in "${SOLS[@]}"; do
		printf "| \`%s\` |" "$s"
		total_score=0
		for st in 0 1 2 3 4 5 6 7; do
			read ok tot < <(awk -v sol="$s" -v st="$st" '
				BEGIN { p = 0; ok = 0; tot = 0 }
				/^=== / {
					gsub(/^=== | ===$/, "")
					p = ($0 == sol)
					next
				}
				p && /^[0-9]+-[0-9]+/ {
					split($1, a, "-")
					if (a[1]+0 == st+0) {
						tot++
						if ($0 ~ /\[Correct\]/) ok++
					}
				}
				END { printf "%d %d", ok, tot }
			' "$LOG")
			if [ "$tot" -eq 0 ]; then
				printf " - |"
			elif [ "$ok" -eq "$tot" ]; then
				score=${SCORES[$st]}
				total_score=$((total_score + score))
				printf " %d/%d (=%d) |" "$ok" "$tot" "$score"
			else
				printf " %d/%d (=0) |" "$ok" "$tot"
			fi
		done
		printf " **%d** |\n" "$total_score"
	done
} > "$OUT"
echo "Wrote $OUT"
