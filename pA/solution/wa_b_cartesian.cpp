// WA #3: counts B as the number of ordered (i, j) pairs with i != j and
// guess[i] == answer[j]. This double-counts whenever a digit repeats in either
// guess or answer, producing a wildly inflated B. Distinct from wa_no_repeat_b
// (which still tracks unique guess positions).
#include <bits/stdc++.h>
using namespace std;

struct Round { int digits[6]; int A, B; };

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
		rounds[i].A = a; rounds[i].B = b;
	}
	int found = -1, count = 0;
	for (int c = 0; c < 1000000; c++) {
		int cd[6];
		toDigits(c, cd);
		bool ok = true;
		for (const auto& r : rounds) {
			int A = 0, B = 0;
			for (int i = 0; i < 6; i++) if (r.digits[i] == cd[i]) A++;
			for (int i = 0; i < 6; i++)
				for (int j = 0; j < 6; j++)
					if (i != j && r.digits[i] == cd[j]) B++;
			if (A != r.A || B != r.B) { ok = false; break; }
		}
		if (ok) { if (count == 0) found = c; count++; if (count > 1) break; }
	}
	if (count == 0) cout << "Impossible\n";
	else if (count > 1) cout << "Ambiguous\n";
	else cout << setfill('0') << setw(6) << found << '\n';
	return 0;
}
