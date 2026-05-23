// Alternative correct solution: maintain a bitset of surviving candidates and
// AND it with the bitset of candidates compatible with each new round.
//
// Used to cross-check the model solution.
#include <bits/stdc++.h>
using namespace std;

static void toDigits(int x, int d[6]) {
	for (int i = 5; i >= 0; i--) { d[i] = x % 10; x /= 10; }
}

static pair<int,int> computeAB(int guess, int answer) {
	int gd[6], ad[6];
	int gc[10] = {0}, ac[10] = {0};
	toDigits(guess, gd); toDigits(answer, ad);
	for (int i = 0; i < 6; i++) { gc[gd[i]]++; ac[ad[i]]++; }
	int A = 0;
	for (int i = 0; i < 6; i++) if (gd[i] == ad[i]) A++;
	int AB = 0;
	for (int d = 0; d < 10; d++) AB += min(gc[d], ac[d]);
	return {A, AB - A};
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n;
	cin >> n;

	bitset<1000000> alive;
	alive.set();

	for (int i = 0; i < n; i++) {
		int g, a, b;
		cin >> g >> a >> b;
		bitset<1000000> mask;
		for (int c = 0; c < 1000000; c++) {
			auto ab = computeAB(g, c);
			if (ab.first == a && ab.second == b) mask.set(c);
		}
		alive &= mask;
	}

	size_t cnt = alive.count();
	if (cnt == 0) cout << "Impossible\n";
	else if (cnt > 1) cout << "Ambiguous\n";
	else {
		int ans = (int)alive._Find_first();
		cout << setfill('0') << setw(6) << ans << '\n';
	}

	return 0;
}
