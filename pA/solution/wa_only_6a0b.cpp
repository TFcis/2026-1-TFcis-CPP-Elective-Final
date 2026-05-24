// WA #4 (anticipated in CLAUDE.md): assumes the answer is uniquely determined
// only when some round has A = 6 and B = 0 (the contestant believes that's
// the only way to pin down the answer). Outputs that round's guess when one
// exists; otherwise outputs "Ambiguous" (never "Impossible", and never derives
// uniqueness from combinations of multiple non-6A0B rounds).
#include <bits/stdc++.h>
using namespace std;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);
	int n; cin >> n;
	string unique_g;
	for (int i = 0; i < n; i++) {
		string g; int a, b;
		cin >> g >> a >> b;
		if (a == 6 && b == 0) unique_g = g;
	}
	if (!unique_g.empty()) cout << unique_g << '\n';
	else                   cout << "Ambiguous\n";
	return 0;
}
