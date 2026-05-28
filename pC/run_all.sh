#!/bin/bash
# Invoke every solution and capture verdicts per test.
cd /mnt/c/Users/YUKI/Documents/2026-1-TFcis-CPP-Elective-Final/pC
mkdir -p invoke_logs
for sol in correct.cpp alt.cpp partial_sub1.cpp partial_sub2.cpp partial_sub3.cpp \
           wa_independent_bfs.cpp wa_steal_later.cpp wa_claim_obstacle.cpp \
           wa_same_round_propagate.cpp wa_first_wins.cpp wa_third_overwrites.cpp \
           wa_adjacent_war.cpp wa_sort_col_first.cpp wa_dim_swap.cpp \
           wa_round_too_few.cpp; do
    echo "=== $sol ==="
    tps invoke "solution/$sol" 2>&1 | tee "invoke_logs/${sol%.cpp}.log" | tail -5
done
