// Partial solution targeting subtask 1: |S| <= 100, deletion only, |A| = 1.
// Assumes A has exactly one character and B is empty; deletes occurrences of A[0].
// Correct on subtask 1; wrong elsewhere because B is ignored and multi-char A is mishandled.
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

	char target = A.empty() ? '\0' : A[0];
	string out;
	out.reserve(S.size());
	for (char c : S) {
		if (c != target) out += c;
	}
	cout << out << '\n';
	return 0;
}
