#!/bin/bash
cd /mnt/c/Users/YUKI/Documents/2026-1-TFcis-CPP-Elective-Final/pA
for f in correct_bitset wa_no_pad wa_pad_bug wa_no_repeat_b wa_ab_swap wa_first_match partial_sub1 tle_slow; do
  echo "=== $f ==="
  tps invoke solution/$f.cpp 2>&1 | tail -10
  echo
done
