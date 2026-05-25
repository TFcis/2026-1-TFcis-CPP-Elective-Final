// Correct (still O(94 * n)). Approach 1 layout, but instead of an O(1) deficit
// update we re-scan all 94 candidate characters every time r advances. This is
// the "suboptimal but still AC" solution from the spec.
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
	int have[256] = {0};
	for (int i = 0; i < m; i++) need[(unsigned char)T[i]]++;

	auto satisfied = [&]() {
		for (int c = 33; c <= 126; c++) {
			if (have[c] < need[c]) return false;
		}
		return true;
	};

	int best = INT_MAX;
	int r = 0;
	for (int l = 0; l < n; l++) {
		while (r < n && !satisfied()) {
			have[(unsigned char)S[r]]++;
			r++;
		}
		if (!satisfied()) break;
		best = min(best, r - l);
		have[(unsigned char)S[l]]--;
	}

	cout << best << "\n";
	return 0;
}
