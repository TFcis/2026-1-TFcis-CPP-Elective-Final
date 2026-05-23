// Partial: hand-crafted to score subtask 1 (n = 1) only.
// For a single round (g, A, B):
//   - If A == 6, the answer is uniquely g.
//   - Otherwise there is at least one candidate but typically many, so output
//     "Ambiguous".
//
// Wrong on every other subtask (will WA on Impossible / unique cases with n > 1).
#include <bits/stdc++.h>
using namespace std;
int main() {
	int n; cin >> n;
	int g, a, b; cin >> g >> a >> b;
	// Consume remaining rounds so reading the file doesn't matter.
	for (int i = 1; i < n; i++) { int x, y, z; cin >> x >> y >> z; }
	if (n == 1 && a == 6) cout << setfill('0') << setw(6) << g << '\n';
	else cout << "Ambiguous\n";
	return 0;
}
