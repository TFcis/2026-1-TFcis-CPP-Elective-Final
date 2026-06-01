// Correct but slow: enumerate every left endpoint l; for each l, binary-search
// the smallest right endpoint r such that the window [l, r] contains T as a
// sub-multiset. The "window valid?" test is monotone in r (a larger window can
// only gain characters), which justifies the binary search. The test itself is
// answered with per-character prefix sums: cnt of char c in [l, r] =
// pre[c][r+1] - pre[c][l]; the window is valid iff that is >= need[c] for every
// distinct character c appearing in T.
//
// Complexity: O(n * log n * D) where D = number of distinct characters in T.
//   - sub1 (D = 1):        ~ n log n          -> AC
//   - sub2 (D <= 2):       ~ 2 n log n        -> AC
//   - sub4 (n <= 2000):    ~ D n log n tiny   -> AC
//   - sub3 / full: D can reach 94 with n = 1e6 -> ~94 * 1e6 * 20 ~ 2e9 -> TLE
//
// Memory: D prefix arrays of length n+1 (built only for chars present in T), so
// small whenever D is small; up to ~94*(1e6+1) ints on full's high-alphabet tests.
#include <bits/stdc++.h>
using namespace std;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n, m;
	cin >> n >> m;
	string S, T;
	cin >> S >> T;

	int need[256] = {0};
	for (int i = 0; i < m; i++) need[(unsigned char)T[i]]++;

	// distinct chars in T
	vector<int> chars;
	for (int c = 0; c < 256; c++) if (need[c] > 0) chars.push_back(c);
	int D = (int)chars.size();

	// prefix sums for each distinct T char: pre[k][i] = #occurrences of chars[k] in S[0..i-1]
	vector<vector<int>> pre(D, vector<int>(n + 1, 0));
	for (int k = 0; k < D; k++) {
		int c = chars[k];
		vector<int>& pk = pre[k];
		for (int i = 0; i < n; i++) {
			pk[i + 1] = pk[i] + ((unsigned char)S[i] == c ? 1 : 0);
		}
	}

	// need values aligned with chars[]
	vector<int> nd(D);
	for (int k = 0; k < D; k++) nd[k] = need[chars[k]];

	// feasible(l, r): does window [l, r] (0-based inclusive) contain T?
	auto feasible = [&](int l, int r) -> bool {
		for (int k = 0; k < D; k++) {
			if (pre[k][r + 1] - pre[k][l] < nd[k]) return false;
		}
		return true;
	};

	int best = INT_MAX;
	for (int l = 0, r = 0; r < n; l++) {
		// smallest r in [l, n-1] with feasible(l, r); binary search.
		int lo = r, hi = n;   // hi == n means "no feasible r for this l"
		while (lo < hi) {
			int mid = lo + (hi - lo) / 2;
			if (feasible(l, mid)) hi = mid;
			else lo = mid + 1;
		}
		if (lo < n) best = min(best, lo - l + 1);
		r = lo;
	}

	cout << best << "\n";
	return 0;
}
