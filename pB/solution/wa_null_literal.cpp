// Anticipated wrong solution (CLAUDE.md bug #5).
//
// Doesn't recognize the "(null)" marker as "empty set"; treats those 6 characters
// as a literal character set. So when A is "(null)", the solution deletes every
// '(', 'n', 'u', 'l', ')' from S; symmetric for B.
//
// Correct on every test where neither A nor B is "(null)". WA on the nullabuse
// tests (subtasks 3, 4, 6, full) and on subtask-2 / subtask-4 / subtask-5 /
// subtask-1 / subtask-3 / full whenever one side is empty and S happens to
// contain one of those 5 chars.
#include <bits/stdc++.h>
using namespace std;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	string S, A, B;
	getline(cin, S);
	getline(cin, A);
	getline(cin, B);
	// Bug: no (null) handling.

	bool inA[128] = {false};
	bool inB[128] = {false};
	for (char c : A) inA[(unsigned char)c] = true;
	for (char c : B) inB[(unsigned char)c] = true;

	string out;
	out.reserve(3 * S.size());
	for (char c : S) {
		unsigned char uc = (unsigned char)c;
		if (inA[uc]) continue;
		if (inB[uc]) { out += 'Q'; out += c; out += 'Q'; }
		else out += c;
	}

	cout << out << '\n';
	return 0;
}
