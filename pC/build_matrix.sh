#!/bin/bash
# Build a human-readable solution-vs-subtask verdict matrix from invoke logs.
cd /mnt/c/Users/YUKI/Documents/2026-1-TFcis-CPP-Elective-Final/pC

OUT=verdict_matrix.txt

# Extract verdict and points for a single subtask from one log.
# Output: "<verdict_short> <pts>/<max>"
get_cell() {
    local log="$1" name="$2"
    local line
    line=$(grep -E "^${name}[[:space:]]" "$log" | head -1)
    [ -z "$line" ] && { echo "?"; return; }
    local v pts
    v=$(echo "$line"  | sed -E 's/^[^[]*\[[^]]*\][[:space:]]*\[([^]]+)\].*/\1/')
    pts=$(echo "$line" | sed -E 's/.*\[([0-9]+\/[0-9]+) pts\].*/\1/')
    local short
    case "$v" in
        *Correct*) short=OK ;;
        *Wrong*)   short=WA ;;
        *Time*)    short=TLE ;;
        *Runtime*) short=RE ;;
        *)         short="$v" ;;
    esac
    echo "$short $pts"
}

{
    echo "pC verdict matrix"
    echo "Generated $(date)"
    echo ""
    echo "Verdict: OK = Correct, WA = Wrong Answer, TLE = Time Limit, RE = Runtime Error"
    echo ""
    fmt="%-32s | %-10s | %-10s | %-10s | %-10s | %-10s | %-10s | %s\n"
    printf "$fmt" "Solution" "samples" "subtask1" "subtask2" "subtask3" "subtask4" "full" "Total"
    echo "---------------------------------+------------+------------+------------+------------+------------+------------+--------"

    for sol in correct.cpp alt.cpp partial_sub1.cpp partial_sub2.cpp partial_sub3.cpp \
               wa_independent_bfs.cpp wa_steal_later.cpp wa_claim_obstacle.cpp \
               wa_same_round_propagate.cpp wa_first_wins.cpp wa_third_overwrites.cpp \
               wa_adjacent_war.cpp wa_sort_col_first.cpp wa_dim_swap.cpp \
               wa_round_too_few.cpp; do
        log=invoke_logs/${sol%.cpp}.log
        [ -f "$log" ] || continue
        c0=$(get_cell "$log" samples)
        c1=$(get_cell "$log" subtask1)
        c2=$(get_cell "$log" subtask2)
        c3=$(get_cell "$log" subtask3)
        c4=$(get_cell "$log" subtask4)
        c5=$(get_cell "$log" full)
        total=$(grep -oE '^[0-9]+/100 pts' "$log" | head -1)
        printf "$fmt" "$sol" "$c0" "$c1" "$c2" "$c3" "$c4" "$c5" "$total"
    done
} > "$OUT"
cat "$OUT"
