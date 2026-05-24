// Partial solution: correctly handles replacement for any B, but ignores A entirely.
// Targets subtasks 2 and 4 (replacement only). Wrong wherever an A deletion is required.
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

	bool inB[128] = {false};
	for (char c : B) inB[(unsigned char)c] = true;

	string out;
	out.reserve(3 * S.size());
	for (char c : S) {
		if (inB[(unsigned char)c]) {
			out += 'Q'; out += c; out += 'Q';
		} else {
			out += c;
		}
	}
	cout << out << '\n';
	return 0;
}
