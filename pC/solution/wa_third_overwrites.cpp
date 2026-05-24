// WA: war detection is bugged for 3+ simultaneous claimants. Per-round, for
// each empty cell, find the set of slime ids in its 8-neighborhood. If there
// is 1 slime -> claim; if exactly 2 slimes -> war zone; but if 3 or more,
// the cell is mistakenly assigned to the third slime in the (sorted) set
// -- the war flag was supposed to be sticky, but the code overwrites it on
// the third encounter.

#include <bits/stdc++.h>
using namespace std;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	const int OBS = -2, WAR = -3, EMP = -1;
	static const int DR[8] = {-1,-1,-1,0,0,1,1,1};
	static const int DC[8] = {-1,0,1,-1,1,-1,0,1};

	int n, m;
	cin >> n >> m;
	vector<string> g(n);
	for (int i = 0; i < n; i++) cin >> g[i];

	vector<pair<int,int>> pos;
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			if (g[i][j] == 'S') pos.emplace_back(i, j);
	int k = (int)pos.size();

	vector<vector<int>> own(n, vector<int>(m, EMP));
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			if (g[i][j] == '#') own[i][j] = OBS;
	for (int s = 0; s < k; s++) own[pos[s].first][pos[s].second] = s;

	while (true) {
		vector<tuple<int,int,int>> changes;
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < m; j++) {
				if (own[i][j] != EMP) continue;
				set<int> srcs;
				for (int d = 0; d < 8; d++) {
					int ni = i + DR[d], nj = j + DC[d];
					if (ni < 0 || ni >= n || nj < 0 || nj >= m) continue;
					int v = own[ni][nj];
					if (v >= 0) srcs.insert(v);
				}
				if (srcs.empty()) continue;
				if ((int)srcs.size() == 1) {
					changes.emplace_back(i, j, *srcs.begin());
				} else if ((int)srcs.size() == 2) {
					changes.emplace_back(i, j, WAR);
				} else {
					// BUG: instead of WAR, assign to the third slime in the set.
					auto it = srcs.begin();
					++it; ++it;
					changes.emplace_back(i, j, *it);
				}
			}
		}
		if (changes.empty()) break;
		for (auto& t : changes) own[get<0>(t)][get<1>(t)] = get<2>(t);
	}

	long long war = 0;
	vector<long long> cnt(k, 0);
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++) {
			if (own[i][j] == WAR) war++;
			else if (own[i][j] >= 0) cnt[own[i][j]]++;
		}

	cout << war << "\n";
	for (int s = 0; s < k; s++) cout << cnt[s] << "\n";
	return 0;
}
