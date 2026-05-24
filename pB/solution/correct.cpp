// Model solution.
// O(n) single linear scan with a 128-entry action lookup table:
//   action[c] = 0 keep, 1 delete, 2 replace with QcQ.
// Reading the literal string "(null)" as the empty set is the only special-case parse.
// Inserted Q's are emitted directly to the output buffer and never re-examined,
// so they are immune to deletion/replacement by construction.
#include <bits/stdc++.h>
using namespace std;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	string S, A, B;
	getline(cin, S);
	getline(cin, A);
	getline(cin, B);

	if (A == "(null)") A.clear();
	if (B == "(null)") B.clear();

	int action[128] = {0};
	for (char c : A) action[(unsigned char)c] = 1;
	for (char c : B) action[(unsigned char)c] = 2;

	string out;
	out.reserve(3 * S.size());
	for (char c : S) {
		int a = action[(unsigned char)c];
		if (a == 1) continue;
		if (a == 2) { out += 'Q'; out += c; out += 'Q'; }
		else out += c;
	}

	cout << out << '\n';
	return 0;
}
