// Partial solution targeting subtask 2: |S| <= 100, replacement only, |B| = 1.
// Assumes B has exactly one character and A is empty; replaces occurrences of B[0]
// with Q + B[0] + Q. Wrong elsewhere: A is ignored and multi-char B is mishandled.
#include <bits/stdc++.h>
using namespace std;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	string S, A, B;
	getline(cin, S);
	getline(cin, A);
	getline(cin, B);

	if (B == "(null)") B.clear();

	char target = B.empty() ? '\0' : B[0];
	string out;
	out.reserve(3 * S.size());
	for (char c : S) {
		if (c == target) { out += 'Q'; out += c; out += 'Q'; }
		else out += c;
	}
	cout << out << '\n';
	return 0;
}
