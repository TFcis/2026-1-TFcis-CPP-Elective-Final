// Partial: only handles cases with no obstacles. Falls back to zeros otherwise.
// Uses the correct scan-based simultaneous simulation.

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
	bool hasObs = false;
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++) {
			if (g[i][j] == 'S') pos.emplace_back(i, j);
			if (g[i][j] == '#') hasObs = true;
		}
	int k = (int)pos.size();

	if (hasObs) {
		cout << 0 << "\n";
		for (int s = 0; s < k; s++) cout << 0 << "\n";
		return 0;
	}

	vector<vector<int>> own(n, vector<int>(m, EMP));
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
	(void)OBS;
	return 0;
}
