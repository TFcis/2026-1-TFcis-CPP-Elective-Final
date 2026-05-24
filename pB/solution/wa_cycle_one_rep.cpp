// Anticipated wrong solution (CLAUDE.md bug #6).
//
// Models a contestant who only knows an O(n^2) string::erase/insert algorithm.
// To salvage the cyclic subtask, they detect the period p with the (buggy)
// one-repetition check, apply the slow algorithm to T = S[0..p-1] (tiny, ~8
// chars), and emit the per-block result n/p times.
//
// Bug: a single-repetition match doesn't prove p is the true period. True
// T = "ABCABCA" (length 7) matches the p=3 one-rep check; the fast-path then
// emits a wrong per-block result repeated across the whole string.
//
// On non-cyclic inputs the check usually doesn't fire, so the algorithm falls
// back to the O(n^2) transform on the full S -- AC for small n, TLE for large.
#include <bits/stdc++.h>
using namespace std;

// O(n^2) in-place transform: same engine as tle_quadratic.cpp.
static void transform(string& s, const bool inA[128], const bool inB[128]) {
	size_t i = 0;
	while (i < s.size()) {
		unsigned char c = (unsigned char)s[i];
		if (inA[c]) {
			s.erase(i, 1);
		} else if (inB[c]) {
			s.insert(i, 1, 'Q');
			s.insert(i + 2, 1, 'Q');
			i += 3;
		} else {
			i++;
		}
	}
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	string S, A, B;
	getline(cin, S);
	getline(cin, A);
	getline(cin, B);

	if (A == "(null)") A.clear();
	if (B == "(null)") B.clear();

	bool inA[128] = {false};
	bool inB[128] = {false};
	for (char c : A) inA[(unsigned char)c] = true;
	for (char c : B) inB[(unsigned char)c] = true;

	int n = (int)S.size();
	int p = -1;
	for (int q = 1; q <= 8 && 2 * q <= n; q++) {
		if (n % q != 0) continue;
		bool ok = true;
		for (int i = 0; i < q; i++) {
			if (S[i] != S[i + q]) { ok = false; break; }
		}
		if (ok) { p = q; break; }
	}

	string out;
	if (p != -1) {
		string block = S.substr(0, p);
		transform(block, inA, inB);  // p <= 8, so this is essentially free
		int k = n / p;
		out.reserve(block.size() * (size_t)k);
		for (int i = 0; i < k; i++) out += block;
	} else {
		transform(S, inA, inB);  // O(n^2) on the full string -- TLE for large n
		out = std::move(S);
	}

	cout << out << '\n';
	return 0;
}
