// Approach 1 shape but the "is window valid?" check scans all m chars of T each
// time r advances -> O(n * m). AC on small n (sub4) and on tiny m (sub2 with
// m=2), TLE elsewhere.
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
	int have[256] = {0};

	auto valid = [&]() {
		int tmp[256] = {0};
		for (int i = 0; i < m; i++) tmp[(unsigned char)T[i]]++;
		for (int c = 0; c < 256; c++) {
			if (have[c] < tmp[c]) return false;
		}
		return true;
	};

	int best = INT_MAX;
	int r = 0;
	for (int l = 0; l < n; l++) {
		while (r < n && !valid()) {
			have[(unsigned char)S[r]]++;
			r++;
		}
		if (!valid()) break;
		best = min(best, r - l);
		have[(unsigned char)S[l]]--;
	}

	cout << best << "\n";
	return 0;
}
