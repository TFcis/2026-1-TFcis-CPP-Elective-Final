// WA #4: never outputs "Impossible". Confuses the no-candidate and
// multi-candidate cases and prints "Ambiguous" in both, so it WAs on every
// genuinely impossible test.
#include <bits/stdc++.h>
using namespace std;

struct Round { int digits[6]; int counts[10]; int A, B; };

static void toDigits(int x, int d[6]) {
	for (int i = 5; i >= 0; i--) { d[i] = x % 10; x /= 10; }
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);
	int n; cin >> n;
	vector<Round> rounds(n);
	for (int i = 0; i < n; i++) {
		int g, a, b; cin >> g >> a >> b;
		toDigits(g, rounds[i].digits);
		for (int j = 0; j < 10; j++) rounds[i].counts[j] = 0;
		for (int j = 0; j < 6; j++) rounds[i].counts[rounds[i].digits[j]]++;
		rounds[i].A = a; rounds[i].B = b;
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
		if (ok) { if (count == 0) found = c; count++; if (count > 1) break; }
	}
	if (count == 0)       cout << "Ambiguous\n";  // BUG: should be Impossible
	else if (count > 1)   cout << "Ambiguous\n";
	else                  cout << setfill('0') << setw(6) << found << '\n';
	return 0;
}
