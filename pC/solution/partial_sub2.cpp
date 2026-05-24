// Partial: only handles subtask 2 (n = 1 and no obstacles). Falls back to zeros
// for any other shape. 1D simultaneous simulation (no 8-direction needed since
// up/down rows don't exist).

#include <bits/stdc++.h>
using namespace std;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n, m;
	cin >> n >> m;
	vector<string> g(n);
	for (int i = 0; i < n; i++) cin >> g[i];

	vector<pair<int,int>> pos;
	bool hasObs = false;
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++) {
			if (g[i][j] == 'S') pos.emplace_back(i, j);
			if (g[i][j] == '#') hasObs = true;
		}
	int k = (int)pos.size();

	if (n != 1 || hasObs) {
		cout << 0 << "\n";
		for (int s = 0; s < k; s++) cout << 0 << "\n";
		return 0;
	}

	const int EMP = -1, WAR = -3;
	vector<int> own(m, EMP);
	for (int s = 0; s < k; s++) own[pos[s].second] = s;
	long long war = 0;
	vector<long long> cnt(k, 1);

	while (true) {
		vector<int> targets(m, -2);
		bool any = false;
		for (int j = 0; j < m; j++) {
			if (own[j] != EMP) continue;
			int first = -1;
			bool multi = false;
			for (int dj = -1; dj <= 1; dj += 2) {
				int nj = j + dj;
				if (nj < 0 || nj >= m) continue;
				int v = own[nj];
				if (v < 0) continue;
				if (first == -1) first = v;
				else if (first != v) multi = true;
			}
			if (first == -1) continue;
			targets[j] = multi ? WAR : first;
			any = true;
		}
		if (!any) break;
		for (int j = 0; j < m; j++) {
			if (targets[j] == -2) continue;
			if (targets[j] == WAR) { own[j] = WAR; war++; }
			else { own[j] = targets[j]; cnt[targets[j]]++; }
		}
	}

	cout << war << "\n";
	for (int s = 0; s < k; s++) cout << cnt[s] << "\n";
	return 0;
}
