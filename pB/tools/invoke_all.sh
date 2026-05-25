#!/bin/bash
# Invoke every solution and capture results to /tmp/pB-invoke.log.
# Run from anywhere; resolves the problem dir as ../ relative to this script.
set -u
cd "$(dirname "$0")/.."
OUT=${1:-/tmp/pB-invoke.log}
SOLS=(correct.cpp alt.cpp partial_sub1.cpp partial_sub2.cpp partial_del_only.cpp partial_rep_only.cpp partial_cyclic.cpp tle_quadratic.cpp wa_replace_then_delete.cpp wa_null_literal.cpp wa_cycle_one_rep.cpp wa_null_s.cpp)
{
	for s in "${SOLS[@]}"; do
		echo "=== $s ==="
		tps invoke "solution/$s" 2>&1
		echo
	done
} > "$OUT" 2>&1
echo "Wrote $OUT"
