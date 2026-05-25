#!/usr/bin/env bash
set -u
cd "$(dirname "$0")/.."
sols=(correct_recheck correct_approach2 partial_sub1 partial_sub2 brute_sub4 tle_check_m tle_reset_r wa_output_m wa_output_n wa_subsequence wa_max_min)
for s in "${sols[@]}"; do
	echo "=== $s ==="
	tps invoke "solution/${s}.cpp" 2>&1 | tee "runlogs/invoke_${s}.log" | tail -90
done
