// Targets sub1 only. Reads T's character, finds the first and last positions of
// it in S, returns the span. Correct exactly when T is m copies of one char and
// S has exactly m copies of that char -- sub1's constraint. On other subtasks it
// typically outputs a too-small answer (because the window also has to contain
// the *other* required characters), so WA.
#include <bits/stdc++.h>
using namespace std;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);
	int n, m;
	cin >> n >> m;
	string S, T;
	cin >> S >> T;

	char target = T[0];
	int first = -1, last = -1;
	for (int i = 0; i < n; i++) {
		if (S[i] == target) {
			if (first == -1) first = i;
			last = i;
		}
	}
	if (first == -1) {
		cout << n << "\n";
	} else {
		cout << (last - first + 1) << "\n";
	}
	return 0;
}
