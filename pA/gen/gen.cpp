#include <bits/stdc++.h>
#include "testlib.h"
using namespace std;

// 1A2B (Bulls and Cows) test data generator.
//
// Usage modes (first argv after the binary name):
//   n1                      n=1, random guess against a random secret answer
//   n1-self                 n=1, guess equals answer (6A0B; answer is unique)
//   unique-same  <digit> [min_rounds]
//                           answer = digit repeated 6 times; rounds make it unique
//   unique-lead0 [min_rounds]
//                           answer in [0, 9999] (first 2 digits are 0); rounds make it unique
//   unique       [min_rounds]
//                           random answer in [0, 999999]; rounds make it unique
//   ambiguous    <num_rounds>
//                           random secret + num_rounds rounds; retried until >1 candidate
//   impossible   [min_rounds]
//                           rounds narrow to a unique answer X, then add (X, 5, 0) to contradict
//
// Extra trailing args (after the last consumed one) are ignored by the mode logic
// but still feed the random seed via testlib's registerGen(argc, argv, ...).

static const int TOTAL = 1000000;

static pair<int,int> computeAB(int guess, int answer) {
	int gd[6], ad[6];
	int gc[10] = {0}, ac[10] = {0};
	int g = guess, a = answer;
	for (int i = 5; i >= 0; i--) {
		gd[i] = g % 10; ad[i] = a % 10;
		g /= 10; a /= 10;
	}
	for (int i = 0; i < 6; i++) { gc[gd[i]]++; ac[ad[i]]++; }
	int A = 0;
	for (int i = 0; i < 6; i++) if (gd[i] == ad[i]) A++;
	int AB = 0;
	for (int d = 0; d < 10; d++) AB += min(gc[d], ac[d]);
	return make_pair(A, AB - A);
}

static vector<int> filterCandidates(const vector<int>& cands, int guess, int A, int B) {
	vector<int> nxt;
	nxt.reserve(cands.size());
	for (size_t i = 0; i < cands.size(); i++) {
		pair<int,int> ab = computeAB(guess, cands[i]);
		if (ab.first == A && ab.second == B) nxt.push_back(cands[i]);
	}
	return nxt;
}

static void printRounds(const vector<tuple<int,int,int> >& rounds) {
	cout << rounds.size() << '\n';
	for (size_t i = 0; i < rounds.size(); i++) {
		cout << get<0>(rounds[i]) << ' '
		     << get<1>(rounds[i]) << ' '
		     << get<2>(rounds[i]) << '\n';
	}
}

// Returns min(actual, limit) where actual is the number of 6-digit
// candidates consistent with `rounds`. Stops counting once `limit` is reached.
static int countCandidates(const vector<tuple<int,int,int> >& rounds, int limit) {
	int cnt = 0;
	for (int c = 0; c < TOTAL && cnt < limit; c++) {
		bool ok = true;
		for (size_t i = 0; i < rounds.size(); i++) {
			pair<int,int> ab = computeAB(get<0>(rounds[i]), c);
			if (ab.first != get<1>(rounds[i]) || ab.second != get<2>(rounds[i])) { ok = false; break; }
		}
		if (ok) cnt++;
	}
	return cnt;
}

// Builds a round list that narrows the candidate set to exactly {answer}.
// At least `min_rounds` rounds will be emitted (padding with extra random-but-consistent
// rounds if uniqueness is reached early). Caps total rounds at `max_rounds`.
static vector<tuple<int,int,int> > makeUnique(int answer, int min_rounds, int max_rounds) {
	vector<tuple<int,int,int> > rounds;
	vector<int> cands(TOTAL);
	iota(cands.begin(), cands.end(), 0);

	while ((int)rounds.size() < max_rounds &&
	       ((int)cands.size() > 1 || (int)rounds.size() < min_rounds)) {
		int g;
		if ((int)cands.size() > 1 && (int)cands.size() <= 50) {
			// Smart pick: try several trial guesses (including existing candidates,
			// which are guaranteed to distinguish themselves from others) and choose
			// the one that shrinks the candidate set the most.
			int bestG = (int)rnd.next(0, 999999);
			int bestSize = INT_MAX;
			vector<int> trials = cands;
			for (int t = 0; t < 30; t++) trials.push_back((int)rnd.next(0, 999999));
			for (size_t i = 0; i < trials.size(); i++) {
				pair<int,int> ab_ans = computeAB(trials[i], answer);
				int sz = 0;
				for (size_t j = 0; j < cands.size(); j++) {
					pair<int,int> ab = computeAB(trials[i], cands[j]);
					if (ab.first == ab_ans.first && ab.second == ab_ans.second) sz++;
				}
				if (sz < bestSize) {
					bestSize = sz;
					bestG = trials[i];
				}
			}
			g = bestG;
		} else {
			g = (int)rnd.next(0, 999999);
		}
		pair<int,int> ab = computeAB(g, answer);
		rounds.push_back(make_tuple(g, ab.first, ab.second));
		cands = filterCandidates(cands, g, ab.first, ab.second);
	}
	return rounds;
}

int main(int argc, char* argv[]) {
	registerGen(argc, argv, 1);
	// about testlib, see https://codeforces.com/blog/entry/18291

	if (argc < 2) {
		cerr << "Usage: gen <mode> [args...]\n";
		return 1;
	}

	string mode = argv[1];

	if (mode == "n1") {
		int answer = (int)rnd.next(0, 999999);
		int g = (int)rnd.next(0, 999999);
		pair<int,int> ab = computeAB(g, answer);
		cout << 1 << '\n' << g << ' ' << ab.first << ' ' << ab.second << '\n';
	} else if (mode == "n1-self") {
		int answer = (int)rnd.next(0, 999999);
		cout << 1 << '\n' << answer << " 6 0\n";
	} else if (mode == "unique-same") {
		if (argc < 3) { cerr << "unique-same requires digit\n"; return 1; }
		int d = atoi(argv[2]);
		if (d < 0 || d > 9) { cerr << "digit must be 0..9\n"; return 1; }
		int min_rounds = (argc >= 4) ? atoi(argv[3]) : 0;
		if (min_rounds < 0 || min_rounds > 255) min_rounds = 0;
		int answer = d * 111111;
		printRounds(makeUnique(answer, min_rounds, 255));
	} else if (mode == "unique-lead0") {
		int min_rounds = (argc >= 3) ? atoi(argv[2]) : 0;
		if (min_rounds < 0 || min_rounds > 255) min_rounds = 0;
		int answer = (int)rnd.next(0, 9999);
		printRounds(makeUnique(answer, min_rounds, 255));
	} else if (mode == "unique") {
		int min_rounds = (argc >= 3) ? atoi(argv[2]) : 0;
		if (min_rounds < 0 || min_rounds > 255) min_rounds = 0;
		int answer = (int)rnd.next(0, 999999);
		printRounds(makeUnique(answer, min_rounds, 255));
	} else if (mode == "ambiguous") {
		if (argc < 3) { cerr << "ambiguous requires num_rounds\n"; return 1; }
		int num_rounds = atoi(argv[2]);
		if (num_rounds < 1 || num_rounds > 50) num_rounds = 3;
		for (int retries = 0; retries < 10; retries++) {
			int answer = (int)rnd.next(0, 999999);
			vector<tuple<int,int,int> > rounds;
			for (int i = 0; i < num_rounds; i++) {
				int g = (int)rnd.next(0, 999999);
				pair<int,int> ab = computeAB(g, answer);
				rounds.push_back(make_tuple(g, ab.first, ab.second));
			}
			if (countCandidates(rounds, 2) > 1) {
				printRounds(rounds);
				return 0;
			}
		}
		// Fallback: should be rare; emit a consistent (possibly unique) test.
		int answer = (int)rnd.next(0, 999999);
		printRounds(makeUnique(answer, num_rounds, 255));
	} else if (mode == "impossible") {
		int min_rounds = (argc >= 3) ? atoi(argv[2]) : 0;
		if (min_rounds < 0 || min_rounds > 253) min_rounds = 0;
		int answer = (int)rnd.next(0, 999999);
		vector<tuple<int,int,int> > rounds = makeUnique(answer, min_rounds, 254);
		// makeUnique should leave only {answer}; adding (answer, 5, 0) excludes it,
		// so no 6-digit number satisfies all rounds -> Impossible.
		rounds.push_back(make_tuple(answer, 5, 0));
		printRounds(rounds);
	} else {
		cerr << "Unknown mode: " << mode << "\n";
		return 1;
	}

	return 0;
}
