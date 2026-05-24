// Partial: only handles single-slime (subtask 1). For k > 1, returns zeros.
// Approach: BFS the one slime through non-obstacle cells, count reachable cells.

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
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			if (g[i][j] == 'S') pos.emplace_back(i, j);
	int k = (int)pos.size();

	if (k != 1) {
		cout << 0 << "\n";
		for (int s = 0; s < k; s++) cout << 0 << "\n";
		return 0;
	}

	vector<vector<int>> vis(n, vector<int>(m, 0));
	queue<pair<int,int>> q;
	q.push(pos[0]);
	vis[pos[0].first][pos[0].second] = 1;
	long long cnt = 0;
	while (!q.empty()) {
		auto p = q.front(); q.pop();
		cnt++;
		int r = p.first, c = p.second;
		for (int dr = -1; dr <= 1; dr++) {
			for (int dc = -1; dc <= 1; dc++) {
				if (dr == 0 && dc == 0) continue;
				int nr = r + dr, nc = c + dc;
				if (nr < 0 || nr >= n || nc < 0 || nc >= m) continue;
				if (g[nr][nc] == '#') continue;
				if (vis[nr][nc]) continue;
				vis[nr][nc] = 1;
				q.push({nr, nc});
			}
		}
	}
	cout << 0 << "\n";
	cout << cnt << "\n";
	return 0;
}
