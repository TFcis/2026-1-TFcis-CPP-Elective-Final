#!/usr/bin/env bash
# Invoke 4 brand-new solutions on ALL tests; invoke 11 existing solutions on
# only the new tests and APPEND to their existing logs. This honors the
# CLAUDE.md rule "avoid regenerating unchanged subtasks" and "solutions should
# consult logs to avoid rerunning unchanged subtasks".
set -u
cd "$(dirname "$0")/.."

NEW_TESTS="1-11,1-12,3-16,3-17,3-18,5-26,5-27,5-28,5-29,5-30,5-31,5-32,5-33,5-34,5-35"

NEW_SOLS=(tle_reset_break tle_prefilter tle_reset_break_rev tle_prefilter_rev)
EXISTING_SOLS=(correct_recheck correct_approach2 partial_sub1 partial_sub2 brute_sub4 tle_check_m tle_reset_r wa_output_m wa_output_n wa_subsequence wa_max_min)

echo "==== invoking 4 new solutions on ALL tests ===="
for s in "${NEW_SOLS[@]}"; do
	echo "--- $s (full) ---"
	tps invoke "solution/${s}.cpp" 2>&1 | tee "runlogs/invoke_${s}.log" | tail -6
done

echo "==== re-invoking 11 existing solutions on NEW tests only ===="
for s in "${EXISTING_SOLS[@]}"; do
	echo "--- $s (delta: $NEW_TESTS) ---"
	{
		echo ""
		echo "# === appended delta from $(date -u +%FT%TZ): -t '${NEW_TESTS}' ==="
		tps invoke -t "${NEW_TESTS}" "solution/${s}.cpp" 2>&1
	} | tee -a "runlogs/invoke_${s}.log" | tail -6
done

echo "==== done ===="
