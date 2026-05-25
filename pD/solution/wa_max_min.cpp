// Uses the Approach 2 data structure v[c], computes for each required char its
// own minimum window (min over j of v[c][j + need[c] - 1] - v[c][j] + 1), then
// takes the MAX across chars. Wrong: the per-char min-windows may sit at very
// different positions, so the actual answer can be much larger than this max.
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

	int ans = 0;
	for (int c = 33; c <= 126; c++) {
		if (need[c] == 0) continue;
		int best = INT_MAX;
		int a = need[c];
		for (int j = 0; (int)j + a - 1 < (int)v[c].size(); j++) {
			best = min(best, v[c][j + a - 1] - v[c][j] + 1);
		}
		ans = max(ans, best);
	}
	cout << ans << "\n";
	return 0;
}
