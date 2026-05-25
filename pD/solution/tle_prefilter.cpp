// AWS #6 (forward): Pre-filter S to keep only positions whose character is in
// T (record original indices so window length is computed against the original
// S). Then apply AWS #5's reset-r + early-break on the filtered array. Faster
// in practice when T uses few chars and S has lots of irrelevant filler, but
// still O(n^2) worst case (when ALL of S consists of T's chars).
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
