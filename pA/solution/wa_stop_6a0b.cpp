// WA #5 (anticipated in CLAUDE.md): stops reading further rounds the moment
// it sees A = 6 and B = 0; immediately prints that guess as the answer. A
// later round that contradicts (e.g. the "(answer, 5, 0)" tail of our
// impossible tests) is never examined, so impossible cases are silently
// reported as a unique answer.
#include <bits/stdc++.h>
using namespace std;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);
	int n; cin >> n;
	for (int i = 0; i < n; i++) {
		string g; int a, b;
		cin >> g >> a >> b;
		if (a == 6 && b == 0) {
			cout << g << '\n';
			return 0;
		}
	}
	cout << "Ambiguous\n";
	return 0;
}
