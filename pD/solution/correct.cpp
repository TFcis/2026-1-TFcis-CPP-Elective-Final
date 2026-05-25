// Model solution -- Approach 1: two-pointer sliding window with a "deficit"
// counter. For each left endpoint l we advance r forward as long as the window
// is short on some character, then record (r - l) and shrink from the left.
// Each index of S is touched at most twice -> O(n + m).
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
	int deficit = 0;
	for (int i = 0; i < m; i++) {
		need[(unsigned char)T[i]]++;
		deficit++;
	}

	int best = INT_MAX;
	int r = 0;
	for (int l = 0; l < n; l++) {
		while (r < n && deficit > 0) {
			unsigned char c = (unsigned char)S[r];
			if (have[c] < need[c]) deficit--;
			have[c]++;
			r++;
		}
		if (deficit > 0) break;        // cannot extend; no l' >= l works either
		best = min(best, r - l);
		unsigned char cl = (unsigned char)S[l];
		have[cl]--;
		if (have[cl] < need[cl]) deficit++;
	}

	cout << best << "\n";
	return 0;
}
