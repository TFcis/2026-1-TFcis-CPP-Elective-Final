// Approach 1 but the right pointer is RESET to l + m - 1 for every new l, then
// walked forward each time. Degenerates to O(n^2). AC sub4 (n^2 = 4*10^6), TLE
// for n=10^6 subtasks.
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
		int deficit = 0;
		for (int c = 33; c <= 126; c++) if (need[c] > 0) deficit++;
		// initial fill [l, l+m-1]
		for (int i = l; i < l + m; i++) {
			unsigned char c = (unsigned char)S[i];
			if (have[c] < need[c] && have[c] + 1 == need[c]) deficit--;
			else if (have[c] < need[c]) {} // still short
			have[c]++;
		}
		// re-derive deficit from scratch to be safe
		deficit = 0;
		for (int c = 33; c <= 126; c++) {
			if (have[c] < need[c]) deficit++;
		}
		int r = l + m - 1;
		while (r < n && deficit > 0) {
			r++;
			if (r >= n) break;
			unsigned char c = (unsigned char)S[r];
			if (have[c] + 1 == need[c]) deficit--;
			have[c]++;
		}
		if (deficit == 0) best = min(best, r - l + 1);
	}

	cout << best << "\n";
	return 0;
}
