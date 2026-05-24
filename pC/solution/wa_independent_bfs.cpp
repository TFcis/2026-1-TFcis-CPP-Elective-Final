// WA approach: independent BFS per slime through '.' and 'S' cells
// (obstacles block). For each cell, find the slime with the minimum BFS
// distance; if a unique slime wins, claim it; if multiple slimes tie at the
// minimum distance, mark as war zone.
//
// Bug: this approach treats every slime's BFS as if it can fill space freely.
// In reality, when other slimes block the path mid-expansion (e.g., the
// pattern `#.S` / `...` / `#.S` from the problem notes), the cell that would
// be a war zone under correct simulation may actually be unreachable by the
// blocked slime — yet this approach still records the BFS distance and marks
// a false war zone.

#include <bits/stdc++.h>
using namespace std;

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

	vector<vector<vector<int>>> dist(k, vector<vector<int>>(n, vector<int>(m, -1)));
	for (int s = 0; s < k; s++) {
		queue<pair<int,int>> q;
		dist[s][pos[s].first][pos[s].second] = 0;
		q.push(pos[s]);
		while (!q.empty()) {
			auto p = q.front(); q.pop();
			int r = p.first, c = p.second;
			for (int d = 0; d < 8; d++) {
				int nr = r + DR[d], nc = c + DC[d];
				if (nr < 0 || nr >= n || nc < 0 || nc >= m) continue;
				if (g[nr][nc] == '#') continue;
				if (dist[s][nr][nc] != -1) continue;
				dist[s][nr][nc] = dist[s][r][c] + 1;
				q.push({nr, nc});
			}
		}
	}

	long long war = 0;
	vector<long long> cnt(k, 0);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			if (g[i][j] == '#') continue;
			int bestD = INT_MAX, bestS = -1;
			bool tie = false;
			for (int s = 0; s < k; s++) {
				if (dist[s][i][j] == -1) continue;
				if (dist[s][i][j] < bestD) { bestD = dist[s][i][j]; bestS = s; tie = false; }
				else if (dist[s][i][j] == bestD) tie = true;
			}
			if (bestS == -1) continue;
			if (tie) war++;
			else cnt[bestS]++;
		}
	}

	cout << war << "\n";
	for (int s = 0; s < k; s++) cout << cnt[s] << "\n";
	return 0;
}
