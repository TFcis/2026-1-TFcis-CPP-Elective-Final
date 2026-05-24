// Partial solution: correctly handles deletion for any A, but ignores B entirely.
// Targets subtasks 1 and 3 (deletion only). Wrong wherever a B replacement is required.
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

	bool inA[128] = {false};
	for (char c : A) inA[(unsigned char)c] = true;

	string out;
	out.reserve(S.size());
	for (char c : S) {
		if (!inA[(unsigned char)c]) out += c;
	}
	cout << out << '\n';
	return 0;
}
