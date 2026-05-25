// Targets sub2 only. Hard-codes |T| = 2: looks at T[0] and T[1] (ignores any
// extra T chars), finds the shortest window containing >=1 copy of each (or 2
// copies if T[0] == T[1]). Wrong on every subtask whose T has >2 chars.
#include <bits/stdc++.h>
using namespace std;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);
	int n, m;
	cin >> n >> m;
	string S, T;
	cin >> S >> T;

	char a = T[0], b = T[1];
	int best = INT_MAX;

	if (a == b) {
		int prev = -1;
		for (int i = 0; i < n; i++) {
			if (S[i] == a) {
				if (prev != -1) best = min(best, i - prev + 1);
				prev = i;
			}
		}
	} else {
		int lastA = -1, lastB = -1;
		for (int i = 0; i < n; i++) {
			if (S[i] == a) lastA = i;
			if (S[i] == b) lastB = i;
			if (lastA != -1 && lastB != -1) {
				best = min(best, i - min(lastA, lastB) + 1);
			}
		}
	}

	if (best == INT_MAX) best = n;
	cout << best << "\n";
	return 0;
}
