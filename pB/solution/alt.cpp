// Alternative correct solution. Used to cross-check the model.
// Approach: two-pass linear scan.
//   Pass 1 computes the exact output length, pre-sizing the buffer.
//   Pass 2 fills the buffer by index assignment (no appends, no resizes).
// Same O(n) but distinct in style and memory pattern.
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

	int action[128] = {0};  // 0 keep, 1 delete, 2 replace
	for (char c : A) action[(unsigned char)c] = 1;
	for (char c : B) action[(unsigned char)c] = 2;

	size_t outLen = 0;
	for (char c : S) {
		int a = action[(unsigned char)c];
		if (a == 0) outLen++;
		else if (a == 2) outLen += 3;
	}

	string out(outLen, '\0');
	size_t pos = 0;
	for (char c : S) {
		int a = action[(unsigned char)c];
		if (a == 0) out[pos++] = c;
		else if (a == 2) {
			out[pos++] = 'Q';
			out[pos++] = c;
			out[pos++] = 'Q';
		}
	}

	cout << out << '\n';
	return 0;
}
