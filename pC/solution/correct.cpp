// Model solution for pC: slime expansion simulation.
//
// Approach: simultaneous round-by-round simulation using per-slime frontiers.
// Each round, every cell in a slime's frontier (newly-claimed cells from the
// previous round) tries to claim its 8 empty neighbors. For each empty target
// cell, we record the first slime that reached it; if a second slime also
// targets it in the same round, it becomes a war zone (acts as a wall in
// subsequent rounds). Each empty cell is "touched" at most once over the
// whole simulation, so total work is O(n*m).
//
// Slimes are numbered by row-major scan of the input grid, which is the
// required output ordering (smaller row first; ties by smaller column).

#include <bits/stdc++.h>
using namespace std;

static const int OBS = -2;
static const int WAR = -3;
static const int EMP = -1;

static const int DR[8] = {-1, -1, -1,  0, 0,  1, 1, 1};
static const int DC[8] = {-1,  0,  1, -1, 1, -1, 0, 1};

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

	vector<vector<int>> own(n, vector<int>(m, EMP));
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			if (g[i][j] == '#') own[i][j] = OBS;
	for (int s = 0; s < k; s++) own[pos[s].first][pos[s].second] = s;

	vector<vector<pair<int,int>>> frontier(k);
	for (int s = 0; s < k; s++) frontier[s].push_back(pos[s]);

	vector<long long> cnt(k, 1);
	long long war = 0;

	vector<int> first_src(n * m, -1);
	vector<char> is_war(n * m, 0);
	vector<int> touched;
	touched.reserve(n * m);

	while (true) {
		for (int key : touched) {
			first_src[key] = -1;
			is_war[key] = 0;
		}
		touched.clear();

		for (int s = 0; s < k; s++) {
			for (const auto& p : frontier[s]) {
				int r = p.first, c = p.second;
				for (int d = 0; d < 8; d++) {
					int nr = r + DR[d], nc = c + DC[d];
					if (nr < 0 || nr >= n || nc < 0 || nc >= m) continue;
					if (own[nr][nc] != EMP) continue;
					int key = nr * m + nc;
					if (first_src[key] == -1) {
						first_src[key] = s;
						touched.push_back(key);
					} else if (first_src[key] != s) {
						is_war[key] = 1;
					}
				}
			}
		}

		if (touched.empty()) break;

		vector<vector<pair<int,int>>> nxt(k);
		for (int key : touched) {
			int r = key / m, c = key % m;
			if (is_war[key]) {
				own[r][c] = WAR;
				war++;
			} else {
				int s = first_src[key];
				own[r][c] = s;
				cnt[s]++;
				nxt[s].emplace_back(r, c);
			}
		}
		frontier = std::move(nxt);
	}

	cout << war << "\n";
	for (int s = 0; s < k; s++) cout << cnt[s] << "\n";

	return 0;
}
