#!/usr/bin/env bash
# Invoke the new wa_sub3_bound solution on ALL tests; re-invoke every other
# solution on just the new 3-19 test (appended to their logs). full's test files
# were NOT renumbered (TPS @include is a scoring dependency, not a file copy),
# so existing full logs stay valid and need no re-run.
set -u
cd "$(dirname "$0")/.."

echo "=== wa_sub3_bound (full run) ==="
tps invoke "solution/wa_sub3_bound.cpp" 2>&1 | tee "runlogs/invoke_wa_sub3_bound.log" | grep -E '^(3-|samples|sub|full)' | head -40

OTHERS=(correct_recheck correct_approach2 partial_sub1 partial_sub2 brute_sub4 \
        tle_check_m tle_reset_r tle_reset_break tle_reset_break_rev \
        tle_prefilter tle_prefilter_rev wa_output_m wa_output_n wa_subsequence wa_max_min)

echo "=== existing solutions on 3-19 only (delta) ==="
for s in "${OTHERS[@]}"; do
	{
		echo ""
		echo "# === appended 3-19 delta on $(date -u +%FT%TZ) ==="
		tps invoke -t "3-19" "solution/${s}.cpp" 2>&1
	} | tee -a "runlogs/invoke_${s}.log" | grep -E '^3-19' | sed "s/^/${s}: /"
done
echo "=== done ==="
