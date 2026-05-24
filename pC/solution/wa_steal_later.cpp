// WA approach: process slimes sequentially in id order; each slime does an
// 8-direction BFS through '.' and 'S' cells (obstacles block), updating a
// global (owner, dist) array as follows:
//   - if this slime reaches a cell strictly faster than the current owner,
//     it steals (owner becomes this slime, dist updated);
//   - if this slime reaches a cell at exactly the same dist as the current
//     owner, the cell becomes a war zone.
//
// Bug: it still ignores that other slimes block real paths -- the BFS treats
// any non-'#' cell as freely traversable, so it overestimates reachability
// on inputs where other slimes' fronts (or resulting war zones) cut a slime
// off from cells the BFS thinks it can reach. Counterexample of this kind
// is the `#.S` / `...` / `#.S` pattern from the problem notes.

#include <bits/stdc++.h>
using namespace std;

static const int WAR = -2;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

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

	vector<vector<int>> owner(n, vector<int>(m, -1));
	vector<vector<int>> bd(n, vector<int>(m, INT_MAX));

	for (int s = 0; s < k; s++) {
		queue<pair<int,int>> q;
		int r0 = pos[s].first, c0 = pos[s].second;
		bd[r0][c0] = 0;
		owner[r0][c0] = s;
		q.push({r0, c0});
		while (!q.empty()) {
			auto p = q.front(); q.pop();
			int r = p.first, c = p.second;
			for (int d = 0; d < 8; d++) {
				int nr = r + DR[d], nc = c + DC[d];
				if (nr < 0 || nr >= n || nc < 0 || nc >= m) continue;
				if (g[nr][nc] == '#') continue;
				int nd = bd[r][c] + 1;
				if (nd < bd[nr][nc]) {
					bd[nr][nc] = nd;
					owner[nr][nc] = s;
					q.push({nr, nc});
				} else if (nd == bd[nr][nc] && owner[nr][nc] != s && owner[nr][nc] != WAR) {
					owner[nr][nc] = WAR;
				}
			}
		}
	}

	long long war = 0;
	vector<long long> cnt(k, 0);
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++) {
			if (owner[i][j] == WAR) war++;
			else if (owner[i][j] >= 0) cnt[owner[i][j]]++;
		}

	cout << war << "\n";
	for (int s = 0; s < k; s++) cout << cnt[s] << "\n";
	return 0;
}
