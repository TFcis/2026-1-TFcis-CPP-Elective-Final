// Partial solution targeting subtask 5: assumes S = T repeated k times with |T| <= 8.
// Detects the true period p by checking that S[i] == S[i % p] holds for ALL i
// (not just the first repetition — that's the careful version, used here so the
// fast-path is correct *when applicable*). Processes T once, repeats the
// per-block output k times.
//
// On non-cyclic inputs (no such p in [1,8] dividing n) the algorithm has nothing
// useful to do, so it just emits S unchanged — that is wrong whenever an A or B
// rule would have modified S, which is the expected outcome on subtasks 1-4, 6, full.
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

	int n = (int)S.size();
	int p = -1;
	for (int q = 1; q <= 8 && q <= n; q++) {
		if (n % q != 0) continue;
		bool ok = true;
		for (int i = q; i < n; i++) {
			if (S[i] != S[i % q]) { ok = false; break; }
		}
		if (ok) { p = q; break; }
	}

	string out;
	if (p == -1) {
		// Not cyclic with p in [1, 8]; bail out.
		out = S;
	} else {
		string block;
		block.reserve(3 * p);
		for (int i = 0; i < p; i++) {
			int a = action[(unsigned char)S[i]];
			if (a == 1) continue;
			if (a == 2) { block += 'Q'; block += S[i]; block += 'Q'; }
			else block += S[i];
		}
		int k = n / p;
		out.reserve((size_t)block.size() * (size_t)k);
		for (int i = 0; i < k; i++) out += block;
	}

	cout << out << '\n';
	return 0;
}
