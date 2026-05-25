// Approach 2: for each character c maintain v[c] = sorted list of positions of c
// in S. If T needs a copies of c, the window must contain v[c][j..j+a-1] for some j.
// Advance the leftmost pointer (the char that pins the left edge of the current
// candidate window) one step at a time; each step the left edge weakly moves right
// and we recompute the right edge as the max of v[c][ptr[c] + need[c] - 1] over c.
// Total iterations <= n, each O(94) -> O(94n).
#include <bits/stdc++.h>
using namespace std;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n, m;
	cin >> n >> m;
	string S, T;
	cin >> S >> T;

	vector<vector<int>> v(256);
	for (int i = 0; i < n; i++) v[(unsigned char)S[i]].push_back(i);

	int need[256] = {0};
	for (int i = 0; i < m; i++) need[(unsigned char)T[i]]++;

	int ptr[256] = {0};
	int best = INT_MAX;

	while (true) {
		int l = INT_MAX;
		int r = -1;
		int leftChar = -1;
		bool any = false;
		for (int c = 33; c <= 126; c++) {
			if (need[c] > 0) {
				any = true;
				int lo = v[c][ptr[c]];
				int hi = v[c][ptr[c] + need[c] - 1];
				if (lo < l) { l = lo; leftChar = c; }
				if (hi > r) r = hi;
			}
		}
		if (!any) break;
		best = min(best, r - l + 1);
		ptr[leftChar]++;
		if (ptr[leftChar] + need[leftChar] - 1 >= (int)v[leftChar].size()) break;
	}

	cout << best << "\n";
	return 0;
}
