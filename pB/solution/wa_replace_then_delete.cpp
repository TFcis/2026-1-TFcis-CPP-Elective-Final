// Anticipated wrong solution (CLAUDE.md bug #2: ordering bug).
//
// Two-pass: first replace every B-char in S with QcQ, then delete every A-char
// from the result. When Q is in A, the deletion pass strips the Q's that were
// just inserted by the replacement pass, producing a wrong output.
//
// Coincidentally correct on any test where A is empty, or B is empty, or
// Q is not in A. So it should AC subtasks 1..4 (one of A/B is always empty there)
// and fail on tests in subtasks 5/6/full that have Q in A together with replacements.
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

	bool inA[128] = {false};
	bool inB[128] = {false};
	for (char c : A) inA[(unsigned char)c] = true;
	for (char c : B) inB[(unsigned char)c] = true;

	// Pass 1: replace
	string tmp;
	tmp.reserve(3 * S.size());
	for (char c : S) {
		if (inB[(unsigned char)c]) { tmp += 'Q'; tmp += c; tmp += 'Q'; }
		else tmp += c;
	}
	// Pass 2: delete (also strips inserted Q's if Q is in A)
	string out;
	out.reserve(tmp.size());
	for (char c : tmp) {
		if (!inA[(unsigned char)c]) out += c;
	}

	cout << out << '\n';
	return 0;
}
