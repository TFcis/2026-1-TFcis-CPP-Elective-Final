// AWS #5 (forward): Approach 1 reset-r-per-l, but break early when extending r
// would make the window length exceed the current best (i.e., once a candidate
// length reaches best, abort this l and move on -- a longer window can't beat
// what we already have). Still O(n^2) worst case when the optimal window sits
// far from the start so best decreases slowly; AC on small n and on data where
// best collapses to a small value quickly.
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

	int best = INT_MAX;
	for (int l = 0; l + m - 1 < n; l++) {
		int have[256] = {0};
		for (int i = l; i < l + m; i++) have[(unsigned char)S[i]]++;
		int deficit = 0;
		for (int c = 33; c <= 126; c++) if (have[c] < need[c]) deficit++;
		int r = l + m - 1;
		while (true) {
			if (deficit == 0) {
				best = min(best, r - l + 1);
				break;
			}
			if (r + 1 >= n) break;
			if ((r + 1) - l + 1 > best) break;  // extending can no longer improve
			r++;
			unsigned char c = (unsigned char)S[r];
			if (have[c] + 1 == need[c]) deficit--;
			have[c]++;
		}
	}

	cout << best << "\n";
	return 0;
}
