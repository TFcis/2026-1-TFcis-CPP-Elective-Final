// Misinterprets "subset" as "subsequence in order". Finds the shortest window
// of S that contains T as an ordered subsequence (preserves T's order). This is
// >= the true (multiset-subset) answer, so it's WA whenever T's required order
// forces a longer window.
#include <bits/stdc++.h>
using namespace std;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);
	int n, m;
	cin >> n >> m;
	string S, T;
	cin >> S >> T;

	int best = INT_MAX;
	for (int l = 0; l < n; l++) {
		int j = 0;
		for (int r = l; r < n; r++) {
			if (S[r] == T[j]) {
				j++;
				if (j == m) {
					best = min(best, r - l + 1);
					break;
				}
			}
		}
	}
	if (best == INT_MAX) best = n;
	cout << best << "\n";
	return 0;
}
