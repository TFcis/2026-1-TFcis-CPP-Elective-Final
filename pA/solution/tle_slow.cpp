// TLE: same brute force, but recomputes the per-round digit counts inside the
// inner loop using sprintf, scans positions naively, and never short-circuits
// on the A mismatch. Total work ~10^6 * n * (sprintf + ~80 ops).
#include <bits/stdc++.h>
using namespace std;

int main() {
	int n;
	scanf("%d", &n);
	vector<tuple<int,int,int>> rounds(n);
	for (int i = 0; i < n; i++) {
		int g, a, b;
		scanf("%d %d %d", &g, &a, &b);
		rounds[i] = {g, a, b};
	}
	int found = -1, count = 0;
	for (int c = 0; c < 1000000; c++) {
		char cs[16];
		sprintf(cs, "%06d", c);
		bool ok = true;
		for (const auto& r : rounds) {
			char gs[16];
			sprintf(gs, "%06d", get<0>(r));
			int A = 0;
			for (int i = 0; i < 6; i++) if (gs[i] == cs[i]) A++;
			int B = 0;
			vector<bool> usedG(6, false), usedC(6, false);
			for (int i = 0; i < 6; i++) if (gs[i] == cs[i]) { usedG[i] = usedC[i] = true; }
			for (int i = 0; i < 6; i++) {
				if (usedG[i]) continue;
				for (int j = 0; j < 6; j++) {
					if (usedC[j]) continue;
					if (gs[i] == cs[j]) { B++; usedG[i] = usedC[j] = true; break; }
				}
			}
			if (A != get<1>(r) || B != get<2>(r)) ok = false;
		}
		if (ok) { if (count == 0) found = c; count++; }
	}
	if (count == 0) printf("Impossible\n");
	else if (count > 1) printf("Ambiguous\n");
	else printf("%06d\n", found);
	return 0;
}
