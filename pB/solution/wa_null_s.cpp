// Anticipated wrong solution: copy-pastes the "(null) means empty" handling
// from A/B to S as well. The spec only marks A and B with the (null) sentinel;
// S is always a real, non-empty string (so "(null)" as the literal value of S
// must be treated as 6 actual characters).
//
// Everywhere else this is the correct linear algorithm; the only test cases
// that catch it are the ones where S is exactly the 6-char string "(null)".
#include <bits/stdc++.h>
using namespace std;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	string S, A, B;
	getline(cin, S);
	getline(cin, A);
	getline(cin, B);

	if (S == "(null)") S.clear();  // BUG: should only apply to A and B
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
