#include "testlib.h"
#include <string>
#include <set>
using namespace std;

const int MAXN = 1000000;

// Resolve A/B raw input: the literal 6-char string "(null)" means empty set.
static string resolveAB(const string& raw) {
	return raw == "(null)" ? string() : raw;
}

int main(int argc, char* argv[]) {
	registerValidation(argc, argv);
	// about testlib, see https://codeforces.com/blog/entry/18426

	// Subtask name passed as a positional argument (e.g. "validator subtask3").
	// testlib's registerValidation only consumes --testset/--group/--testOverviewLogFileName,
	// so we read the first non-flag argument as the subtask name.
	string subtask;
	for (int i = 1; i < argc; i++) {
		string a = argv[i];
		if (a.empty()) continue;
		if (a == "--testset" || a == "--group" || a == "--testOverviewLogFileName") {
			i++;
			continue;
		}
		if (!a.empty() && a[0] == '-') continue;
		subtask = a;
		break;
	}

	// All three lines must be non-empty and contain only printable ASCII 33-126
	// (no whitespace). readLine() reads through the trailing '\n' in strict mode.
	// "!" = 0x21 (33), "~" = 0x7e (126).
	string S = inf.readLine("[!-~]{1,1000000}", "S");
	string A_raw = inf.readLine("[!-~]+", "A");
	string B_raw = inf.readLine("[!-~]+", "B");
	inf.readEof();

	int n = (int)S.length();

	string A = resolveAB(A_raw);
	string B = resolveAB(B_raw);

	// Within-A and within-B distinctness.
	{
		set<char> seen;
		for (size_t i = 0; i < A.size(); i++) {
			ensuref(seen.insert(A[i]).second,
				"A contains duplicate character 0x%02X ('%c')",
				(unsigned char)A[i], A[i]);
		}
	}
	{
		set<char> seen;
		for (size_t i = 0; i < B.size(); i++) {
			ensuref(seen.insert(B[i]).second,
				"B contains duplicate character 0x%02X ('%c')",
				(unsigned char)B[i], B[i]);
		}
	}

	// A and B must be disjoint.
	{
		set<char> inA(A.begin(), A.end());
		for (size_t i = 0; i < B.size(); i++) {
			ensuref(inA.find(B[i]) == inA.end(),
				"Character 0x%02X ('%c') appears in both A and B",
				(unsigned char)B[i], B[i]);
		}
	}

	// Per-subtask special constraints.
	int aLen = (int)A.length();
	int bLen = (int)B.length();

	if (subtask == "subtask1") {
		// |S| <= 100, deletion only, |A| = 1
		ensuref(n <= 100, "subtask1 requires |S| <= 100, got %d", n);
		ensuref(bLen == 0, "subtask1 is deletion only, but |B| = %d", bLen);
		ensuref(aLen == 1, "subtask1 requires |A| = 1, got %d", aLen);
	} else if (subtask == "subtask2") {
		// |S| <= 100, replacement only, |B| = 1
		ensuref(n <= 100, "subtask2 requires |S| <= 100, got %d", n);
		ensuref(aLen == 0, "subtask2 is replacement only, but |A| = %d", aLen);
		ensuref(bLen == 1, "subtask2 requires |B| = 1, got %d", bLen);
	} else if (subtask == "subtask3") {
		// Deletion only
		ensuref(bLen == 0, "subtask3 is deletion only, but |B| = %d", bLen);
	} else if (subtask == "subtask4") {
		// Replacement only
		ensuref(aLen == 0, "subtask4 is replacement only, but |A| = %d", aLen);
	} else if (subtask == "subtask5") {
		// S is formed by repeating a length-p string cyclically, p in [1, 8],
		// and p divides n. We accept the input if ANY such p exists.
		bool found = false;
		for (int p = 1; p <= 8 && p <= n; p++) {
			if (n % p != 0) continue;
			bool ok = true;
			for (int i = p; i < n; i++) {
				if (S[i] != S[i % p]) { ok = false; break; }
			}
			if (ok) { found = true; break; }
		}
		ensuref(found,
			"subtask5 requires S to be a repetition of a length-p block with p in [1,8] and p | n");
	} else if (subtask == "subtask6") {
		// |S| <= 2000
		ensuref(n <= 2000, "subtask6 requires |S| <= 2000, got %d", n);
	}
	// "samples", "full", and unrecognized subtasks: only the basic format checks apply.

	return 0;
}
