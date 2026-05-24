// WA: simulation logic is correct, but slime IDs are assigned by sorting
// starting positions by column-first, then row -- the reverse of the
// required (row-first, then column) ordering. The cell counts per slime
// come out swapped on any input where the two sortings differ.

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

	// BUG: sort by (column, row) instead of (row, column).
	sort(pos.begin(), pos.end(), [](const pair<int,int>& a, const pair<int,int>& b) {
		if (a.second != b.second) return a.second < b.second;
		return a.first < b.first;
	});

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
