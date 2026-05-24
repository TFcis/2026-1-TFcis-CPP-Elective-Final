// Correct in logic but O(n^2) in time.
// Modifies S in place using std::string::erase and std::string::insert; each call
// is O(n). The index `i` advances past inserted Q's so they are never re-processed,
// so the output is correct. The cost is quadratic, which is fine up to ~|S| = 2000
// (subtask 6) but TLEs at |S| in the 1e5..1e6 range.
//
// Also serves as the targeted partial solution for subtask 6.
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

	size_t i = 0;
	while (i < S.size()) {
		unsigned char c = (unsigned char)S[i];
		if (inA[c]) {
			S.erase(i, 1);
		} else if (inB[c]) {
			S.insert(i, 1, 'Q');
			S.insert(i + 2, 1, 'Q');
			i += 3;
		} else {
			i++;
		}
	}

	cout << S << '\n';
	return 0;
}
