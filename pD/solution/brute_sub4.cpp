// O(n^2 + n * 94) brute force: for each l, advance r and maintain a count array;
// at each step check whether all 94 chars are satisfied. Designed to AC sub4
// (n <= 2000 -> ~4*10^6 ops) and TLE everything bigger.
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
	for (int l = 0; l < n; l++) {
		int have[256] = {0};
		for (int r = l; r < n; r++) {
			have[(unsigned char)S[r]]++;
			bool ok = true;
			for (int c = 33; c <= 126; c++) {
				if (have[c] < need[c]) { ok = false; break; }
			}
			if (ok) {
				best = min(best, r - l + 1);
				break;
			}
		}
	}

	cout << best << "\n";
	return 0;
}
