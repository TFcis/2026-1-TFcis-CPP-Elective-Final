// WA: doesn't form war zones. When multiple slimes target the same empty cell
// in a round, the first slime evaluated wins; all subsequent claims to that
// cell are silently ignored.

#include <bits/stdc++.h>
using namespace std;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	const int OBS = -2, EMP = -1;
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
		vector<vector<char>> taken(n, vector<char>(m, 0));
		bool any = false;
		// Iterate slimes in id order, expand from every cell owned by this slime.
		// First slime to "land" on an empty cell wins (no war detection).
		for (int s = 0; s < k; s++) {
			for (int i = 0; i < n; i++) {
				for (int j = 0; j < m; j++) {
					if (own[i][j] != s) continue;
					for (int d = 0; d < 8; d++) {
						int ni = i + DR[d], nj = j + DC[d];
						if (ni < 0 || ni >= n || nj < 0 || nj >= m) continue;
						if (own[ni][nj] != EMP) continue;
						if (taken[ni][nj]) continue;
						taken[ni][nj] = 1;
						changes.emplace_back(ni, nj, s);
						any = true;
					}
				}
			}
		}
		if (!any) break;
		for (auto& t : changes) own[get<0>(t)][get<1>(t)] = get<2>(t);
	}

	long long war = 0;
	vector<long long> cnt(k, 0);
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			if (own[i][j] >= 0) cnt[own[i][j]]++;

	cout << war << "\n";
	for (int s = 0; s < k; s++) cout << cnt[s] << "\n";
	return 0;
}
