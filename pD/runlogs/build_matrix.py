#!/usr/bin/env python3
"""Build a human-readable solution x subtask matrix from runlogs/invoke_*.log.

Each invoke_<sol>.log is the captured output of `tps invoke solution/<sol>.cpp`,
which prints one line per test like:

  1-03                Correct                  0.052s

We collapse per-test verdicts to a per-subtask verdict (worst across tests in
the subtask) and emit a markdown matrix to runlogs/result_matrix.md.
"""
import os
import re
import sys
import json
from collections import defaultdict

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
RUNLOGS = os.path.join(ROOT, "runlogs")

# Severity order: higher number = worse. Keys are normalized lower-case.
SEVERITY = {
	"correct":               0,
	"partially correct":     1,
	"wrong answer":          2,
	"time limit exceeded":   3,
	"runtime error":         3,
	"memory limit exceeded": 3,
	"failed":                3,
}
SHORT = {
	"correct":               "AC",
	"partially correct":     "PC",
	"wrong answer":          "WA",
	"time limit exceeded":   "TLE",
	"runtime error":         "RE",
	"memory limit exceeded": "MLE",
	"failed":                "FAIL",
}

def parse_subtasks():
	"""Return (names_in_index_order, {index_value: name})."""
	with open(os.path.join(ROOT, "subtasks.json"), encoding="utf-8") as f:
		j = json.load(f)
	pairs = []
	for name, info in j["subtasks"].items():
		pairs.append((info["index"], name))
	pairs.sort()
	names = [n for _, n in pairs]
	by_idx = {i: n for i, n in pairs}
	return names, by_idx

def parse_invoke_log(path):
	"""Return {test_name: verdict_string_normalized_lower}.

	Lines look like:
	  1-05                sol[FAIL]  3.033     check[SKIP]      0  [Time Limit Exceeded]
	"""
	out = {}
	# capture test name at start, and the trailing bracketed verdict.
	pat = re.compile(r"^(\d+-\d+)\s+.*\[([^\]]+)\]\s*$")
	with open(path, encoding="utf-8", errors="replace") as f:
		for raw in f:
			line = raw.rstrip()
			m = pat.match(line)
			if m:
				out[m.group(1)] = m.group(2).strip().lower()
	return out

def test_subtask_index(test_name):
	"""Test name format '<subtask_index>-<seq>' (e.g. '4-12')."""
	m = re.match(r"^(\d+)-", test_name)
	return int(m.group(1)) if m else None

def main():
	subtasks, idx_to_subtask_by_index_local = parse_subtasks()
	global idx_to_subtask_by_index
	idx_to_subtask_by_index = idx_to_subtask_by_index_local

	# Collect solutions: any invoke_*.log in runlogs/.
	sols = []
	for fn in sorted(os.listdir(RUNLOGS)):
		if fn.startswith("invoke_") and fn.endswith(".log"):
			sols.append(fn[len("invoke_"):-len(".log")])

	# Per (sol, subtask_name) -> worst verdict.
	matrix = {}
	for sol in sols:
		results = parse_invoke_log(os.path.join(RUNLOGS, f"invoke_{sol}.log"))
		# Group by subtask index.
		by_sub = defaultdict(list)
		for test, verdict in results.items():
			idx = test_subtask_index(test)
			if idx is None or idx not in idx_to_subtask_by_index:
				continue
			by_sub[idx].append(verdict)
		for idx, verdicts in by_sub.items():
			worst = max(verdicts, key=lambda v: SEVERITY.get(v, 9))
			# subtask name is derived from index, but the JSON enumeration is
			# already index-sorted so we can use idx_to_name keyed by ordinal.
			# We map idx -> name by matching subtasks[idx_position]. Since the
			# subtasks list is sorted by index in parse_subtasks(), idx itself
			# is the index value, not list position. So:
			matrix[(sol, idx_to_subtask_by_index[idx])] = worst

	# Add the model solution row (assumed all-AC since `tps gen` ran sol[OK] on every test).
	# We synthesize it.
	sols_full = ["correct"] + sols

	# Emit markdown.
	lines = []
	lines.append("# pD Solution x Subtask Result Matrix")
	lines.append("")
	lines.append("Auto-generated from `runlogs/invoke_*.log`.")
	lines.append("")
	header = "| solution \\ subtask |" + "".join(f" {s} |" for s in subtasks)
	sep = "|" + "---|" * (len(subtasks) + 1)
	lines.append(header)
	lines.append(sep)
	for sol in sols_full:
		row = f"| {sol} |"
		for s in subtasks:
			if sol == "correct":
				# Model: AC on every subtask (guaranteed by `tps gen`'s sol[OK]).
				row += " AC |"
			else:
				v = matrix.get((sol, s))
				row += f" {SHORT.get(v, '-')} |"
		lines.append(row)
	lines.append("")
	lines.append("Legend: AC = correct, WA = wrong answer, TLE = time limit, RE = runtime error, PC = partially correct, - = no data.")
	lines.append("")

	out_path = os.path.join(RUNLOGS, "result_matrix.md")
	with open(out_path, "w", encoding="utf-8") as f:
		f.write("\n".join(lines))
	print(f"wrote {out_path}")

if __name__ == "__main__":
	main()
