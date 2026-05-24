// Intended solution: brute force enumerate all 6-digit candidates (000000..999999)
// and check each against every recorded round.
//
// 1A2B rule with possible repeated digits:
//   A = positions where guess[i] == answer[i]
//   A + B = sum over digit d of min(count_d(guess), count_d(answer))
//
// Complexity: O(10^6 * n * (6 + 10)) with early break on A mismatch. ~1.5e9 in the
// absolute worst case (n = 255), well within 3 s with the constant-factor optimisations.
#include <bits/stdc++.h>
using namespace std;

struct Round {
	int digits[6];
	int counts[10];
	int A, B;
};

static void toDigits(int x, int d[6]) {
	for (int i = 5; i >= 0; i--) { d[i] = x % 10; x /= 10; }
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n;
	cin >> n;
	vector<Round> rounds(n);
	for (int i = 0; i < n; i++) {
		// Guess is read as a fixed-length 6-digit string -- the leading-zero
		// representation is part of the contract, so we never round-trip
		// through an int that would drop it.
		string g; int a, b;
		cin >> g >> a >> b;
		for (int j = 0; j < 6; j++) rounds[i].digits[j] = g[j] - '0';
		for (int j = 0; j < 10; j++) rounds[i].counts[j] = 0;
		for (int j = 0; j < 6; j++) rounds[i].counts[rounds[i].digits[j]]++;
		rounds[i].A = a;
		rounds[i].B = b;
	}

	int found = -1, count = 0;
	for (int c = 0; c < 1000000; c++) {
		int cd[6], cc[10] = {0};
		toDigits(c, cd);
		for (int i = 0; i < 6; i++) cc[cd[i]]++;
		bool ok = true;
		for (const auto& r : rounds) {
			int A = 0;
			for (int i = 0; i < 6; i++) if (r.digits[i] == cd[i]) A++;
			if (A != r.A) { ok = false; break; }
			int AB = 0;
			for (int d = 0; d < 10; d++) AB += min(r.counts[d], cc[d]);
			if (AB - A != r.B) { ok = false; break; }
		}
		if (ok) {
			if (count == 0) found = c;
			count++;
			if (count > 1) break;
		}
	}

	if (count == 0) cout << "Impossible\n";
	else if (count > 1) cout << "Ambiguous\n";
	else cout << setfill('0') << setw(6) << found << '\n';

	return 0;
}
