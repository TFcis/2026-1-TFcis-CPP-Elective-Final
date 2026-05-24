// WA: at initialization, the code mistakenly treats any pair of initially
// 8-adjacent slimes as already in a "war" -- it turns both of their starting
// cells into war zones before any expansion. (Confuses "occupies the cell at
// start of round" with "about to expand into that cell".)

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

	// BUG: pre-mark initially adjacent slime cells as war zones.
	vector<vector<char>> turnToWar(n, vector<char>(m, 0));
	for (int s = 0; s < k; s++) {
		int r = pos[s].first, c = pos[s].second;
		for (int d = 0; d < 8; d++) {
			int nr = r + DR[d], nc = c + DC[d];
			if (nr < 0 || nr >= n || nc < 0 || nc >= m) continue;
			if (own[nr][nc] >= 0 && own[nr][nc] != s) {
				turnToWar[r][c] = 1;
				turnToWar[nr][nc] = 1;
			}
		}
	}
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			if (turnToWar[i][j]) own[i][j] = WAR;

	// Then run the otherwise-correct simulation.
	while (true) {
		vector<tuple<int,int,int>> changes;
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < m; j++) {
				if (own[i][j] != EMP) continue;
				int first = -1;
				bool war = false;
				for (int d = 0; d < 8; d++) {
					int ni = i + DR[d], nj = j + DC[d];
					if (ni < 0 || ni >= n || nj < 0 || nj >= m) continue;
					int v = own[ni][nj];
					if (v < 0) continue;
					if (first == -1) first = v;
					else if (first != v) { war = true; break; }
				}
				if (first == -1) continue;
				changes.emplace_back(i, j, war ? WAR : first);
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
