// Same algorithm as tle_prefilter.cpp but reverses S as preprocessing. Helps
// exactly when the unique optimal window sits near the end of the original S;
// after reversal it sits at the start and the early-break collapses inner work
// to O(np * (m + 94)). When the optimal sits in the middle, both forward and
// reverse variants still TLE.
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

	vector<int> idx;
	string Sp;
	idx.reserve(n);
	Sp.reserve(n);
	for (int i = 0; i < n; i++) {
		if (need[(unsigned char)S[i]] > 0) {
			idx.push_back(i);
			Sp.push_back(S[i]);
		}
	}
	int np = (int)Sp.size();

	int best = INT_MAX;
	for (int l = 0; l + m - 1 < np; l++) {
		int have[256] = {0};
		for (int i = l; i < l + m; i++) have[(unsigned char)Sp[i]]++;
		int deficit = 0;
		for (int c = 33; c <= 126; c++) if (have[c] < need[c]) deficit++;
		int r = l + m - 1;
		while (true) {
			if (deficit == 0) {
				best = min(best, idx[r] - idx[l] + 1);
				break;
			}
			if (r + 1 >= np) break;
			if (idx[r + 1] - idx[l] + 1 > best) break;
			r++;
			unsigned char c = (unsigned char)Sp[r];
			if (have[c] + 1 == need[c]) deficit--;
			have[c]++;
		}
	}

	cout << best << "\n";
	return 0;
}
