// WA #3 (anticipated in CLAUDE.md): uses scanf("%i", ...) to read guesses.
// "%i" interprets a number with a leading 0 as octal, so any 6-digit guess
// starting with '0' (and containing only digits 0-7) is parsed to the wrong
// value, e.g. "012345" -> 5349 instead of 12345. Other model logic is
// otherwise identical to correct.cpp.
#include <bits/stdc++.h>
using namespace std;

struct Round { int digits[6]; int counts[10]; int A, B; };

static void toDigits(int x, int d[6]) {
	for (int i = 5; i >= 0; i--) { d[i] = x % 10; x /= 10; }
}

int main() {
	int n;
	scanf("%d", &n);
	vector<Round> rounds(n);
	for (int i = 0; i < n; i++) {
		int g, a, b;
		scanf("%i %d %d", &g, &a, &b);  // BUG: "%i" parses "0..." as octal
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
	if (count == 0) printf("Impossible\n");
	else if (count > 1) printf("Ambiguous\n");
	else printf("%06d\n", found);
	return 0;
}
