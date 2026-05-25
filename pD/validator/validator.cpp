#include "testlib.h"
#include <string>
#include <cstring>
using namespace std;

const int MAXN = 1000000;
const int MAXM = 100000;
const int LO = 33;
const int HI = 126;

static string readFixedLine(int len, const char* name) {
	string s;
	s.resize(len);
	for (int i = 0; i < len; i++) {
		char c = inf.readChar();
		ensuref(((unsigned char)c) >= LO && ((unsigned char)c) <= HI,
			"%s[%d] = ASCII %d, expected in [%d,%d]",
			name, i, (int)(unsigned char)c, LO, HI);
		s[i] = c;
	}
	inf.readEoln();
	return s;
}

int main(int argc, char* argv[]) {
	registerValidation(argc, argv);

	// Pull the subtask name out of argv (it is a positional arg injected by
	// subtask_sensitive_validators: ["validator.cpp {subtask}"]). testlib has
	// already consumed its own --testset/--group/--testOverviewLogFileName flags
	// and their values, but we skip any remaining "--..." flags defensively.
	string subtask;
	for (int i = 1; i < argc; i++) {
		string a = argv[i];
		if (a.size() >= 2 && a[0] == '-' && a[1] == '-') {
			// Either a flag testlib left in, or a flag-with-value.
			if (a == "--testset" || a == "--group"
				|| a == "--testOverviewLogFileName") i++;
			continue;
		}
		subtask = a;
		break;
	}

	int n = inf.readInt(1, MAXN, "n");
	inf.readSpace();
	int m = inf.readInt(1, MAXM, "m");
	inf.readEoln();

	string S = readFixedLine(n, "S");
	string T = readFixedLine(m, "T");

	inf.readEof();

	// Solution-existence: for every char c, count(c, T) <= count(c, S).
	int cntS[256], cntT[256];
	memset(cntS, 0, sizeof(cntS));
	memset(cntT, 0, sizeof(cntT));
	for (int i = 0; i < n; i++) cntS[(unsigned char)S[i]]++;
	for (int i = 0; i < m; i++) cntT[(unsigned char)T[i]]++;
	for (int c = LO; c <= HI; c++) {
		ensuref(cntT[c] <= cntS[c],
			"no solution exists: char '%c' (ASCII %d) appears %d times in T but only %d times in S",
			c, c, cntT[c], cntS[c]);
	}

	// Per-subtask constraints.
	if (subtask.empty() || subtask == "samples" || subtask == "full") {
		// No additional constraints beyond the global format/existence checks.
	} else if (subtask == "sub1") {
		int distinct = 0;
		int uniq = -1;
		for (int c = LO; c <= HI; c++) {
			if (cntT[c] > 0) { distinct++; uniq = c; }
		}
		ensuref(distinct == 1,
			"sub1: T must contain exactly 1 distinct char, found %d distinct",
			distinct);
		ensuref(cntS[uniq] == m,
			"sub1: char '%c' (ASCII %d) must appear in S exactly m=%d times, got %d",
			(char)uniq, uniq, m, cntS[uniq]);
	} else if (subtask == "sub2") {
		ensuref(m == 2, "sub2: m must be 2, got %d", m);
	} else if (subtask == "sub3") {
		for (int i = 1; i < n; i++) {
			ensuref((unsigned char)S[i-1] <= (unsigned char)S[i],
				"sub3: S not non-decreasing at index %d (S[%d]=%d > S[%d]=%d)",
				i, i-1, (int)(unsigned char)S[i-1], i, (int)(unsigned char)S[i]);
		}
		for (int i = 1; i < m; i++) {
			ensuref((unsigned char)T[i-1] <= (unsigned char)T[i],
				"sub3: T not non-decreasing at index %d (T[%d]=%d > T[%d]=%d)",
				i, i-1, (int)(unsigned char)T[i-1], i, (int)(unsigned char)T[i]);
		}
	} else if (subtask == "sub4") {
		ensuref(n <= 2000, "sub4: n must be <= 2000, got %d", n);
	} else {
		ensuref(false, "unknown subtask name: '%s'", subtask.c_str());
	}

	return 0;
}
