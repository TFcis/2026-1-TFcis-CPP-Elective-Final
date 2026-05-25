#!/usr/bin/env bash
# Append per-test results for the 5 new sub2 adversarial tests to each
# solution's existing invoke log. Existing test verdicts on the original 78+15
# tests remain untouched in the logs.
set -u
cd "$(dirname "$0")/.."

NEW_TESTS="2-11,2-12,2-13,2-14,2-15"
SOLS=(correct_recheck correct_approach2 partial_sub1 partial_sub2 brute_sub4 tle_check_m tle_reset_r tle_reset_break tle_reset_break_rev tle_prefilter tle_prefilter_rev wa_output_m wa_output_n wa_subsequence wa_max_min)

for s in "${SOLS[@]}"; do
	echo "--- $s ($NEW_TESTS) ---"
	{
		echo ""
		echo "# === appended adv_sub2 delta on $(date -u +%FT%TZ): -t '${NEW_TESTS}' ==="
		tps invoke -t "${NEW_TESTS}" "solution/${s}.cpp" 2>&1
	} | tee -a "runlogs/invoke_${s}.log" | grep -E "^2-1[12345]\s|sub2" | head -8
done
echo "==== done ===="
