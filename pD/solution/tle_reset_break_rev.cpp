// Same algorithm as tle_reset_break.cpp but reverses S as preprocessing.
// Reversing doesn't change the answer (the multiset condition is symmetric).
// However, if the data places the unique optimal window near the END of S, the
// forward early-break iterates through O(n) l-values before best drops far
// enough for the break to kick in. After reversal that optimal window sits at
// the START, best collapses to m immediately, and every subsequent l breaks
// after one step -> O(n * (m + 94)). Tests where the optimal window is at the
// MIDDLE still TLE either direction.
#include <bits/stdc++.h>
using namespace std;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);
	int n, m;
	cin >> n >> m;
	string S, T;
	cin >> S >> T;
	reverse(S.begin(), S.end());

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
			if ((r + 1) - l + 1 > best) break;
			r++;
			unsigned char c = (unsigned char)S[r];
			if (have[c] + 1 == need[c]) deficit--;
			have[c]++;
		}
	}

	cout << best << "\n";
	return 0;
}
