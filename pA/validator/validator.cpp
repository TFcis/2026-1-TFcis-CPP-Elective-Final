#include "testlib.h"
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

const int MAXN = 255;

struct Round {
	int digits[6];
	int counts[10];
	int a, b;
};

static vector<Round> rounds;

static void toDigits(int x, int d[6]) {
	for (int i = 5; i >= 0; i--) { d[i] = x % 10; x /= 10; }
}

static int parseGuess(const string& s) {
	int x = 0;
	for (size_t i = 0; i < s.size(); i++) x = x * 10 + (s[i] - '0');
	return x;
}

// Returns true iff the 6-digit number `candidate` (with leading zeros) is
// consistent with every recorded round under the 1A2B rule (repeated digits
// matched via min(count_in_guess, count_in_candidate) summed over digits).
static bool consistent(int candidate) {
	int cd[6], cc[10] = {0};
	toDigits(candidate, cd);
	for (int i = 0; i < 6; i++) cc[cd[i]]++;

	for (const auto& r : rounds) {
		int a = 0;
		for (int i = 0; i < 6; i++) if (r.digits[i] == cd[i]) a++;
		if (a != r.a) return false;
		int ab = 0;
		for (int d = 0; d < 10; d++) ab += min(r.counts[d], cc[d]);
		if (ab - a != r.b) return false;
	}
	return true;
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

	int n = inf.readInt(1, MAXN, "n");
	inf.readEoln();

	for (int i = 1; i <= n; i++) {
		// Each guess must be written as exactly 6 decimal digits (leading zeros required).
		string g_str = inf.readToken("[0-9]{6}", "guess_" + to_string(i));
		int guess = parseGuess(g_str);
		inf.readSpace();
		int a = inf.readInt(0, 6, "A_" + to_string(i));
		inf.readSpace();
		int b = inf.readInt(0, 6, "B_" + to_string(i));
		ensuref(a + b <= 6,
			"A + B must be at most 6 at round %d (got %d + %d)", i, a, b);
		inf.readEoln();

		Round r;
		r.a = a; r.b = b;
		toDigits(guess, r.digits);
		for (int j = 0; j < 10; j++) r.counts[j] = 0;
		for (int j = 0; j < 6; j++) r.counts[r.digits[j]]++;
		rounds.push_back(r);
	}

	inf.readEof();

	if (subtask == "subtask1") {
		ensuref(n == 1, "subtask1 requires n = 1, got n = %d", n);
	} else if (subtask == "subtask2" || subtask == "subtask3" ||
	           subtask == "subtask4" || subtask == "subtask5") {
		// subtask2/3/4: answer must be unique.
		// subtask5: answer may not be unique, but at least one candidate must exist.
		bool needUnique = (subtask != "subtask5");
		int total = 0;
		int firstCand = -1;

		for (int cand = 0; cand < 1000000; cand++) {
			if (consistent(cand)) {
				if (total == 0) firstCand = cand;
				total++;
				if (needUnique) {
					if (total > 1) break;
				} else {
					break;
				}
			}
		}

		if (!needUnique) {
			ensuref(total >= 1,
				"subtask5 requires no contradiction, but no candidate is consistent");
		} else {
			ensuref(total == 1,
				"%s requires a unique answer, but found %s candidate(s)",
				subtask.c_str(), (total == 0 ? "0" : "more than 1"));
			int d[6];
			toDigits(firstCand, d);
			if (subtask == "subtask2") {
				for (int i = 1; i < 6; i++) {
					ensuref(d[i] == d[0],
						"subtask2 requires all 6 digits of the answer to be the same, got %06d",
						firstCand);
				}
			} else if (subtask == "subtask3") {
				ensuref(d[0] == 0 && d[1] == 0,
					"subtask3 requires the first 2 digits of the answer to be 0, got %06d",
					firstCand);
			}
			// subtask4: uniqueness alone is sufficient.
		}
	}
	// samples / full / unrecognized: no additional checks beyond basic format.

	return 0;
}
