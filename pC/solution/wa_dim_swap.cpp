// WA: reads the dimensions in the wrong order -- treats the first number as
// m (cols) and the second as n (rows). On square inputs this happens to give
// the correct answer; on non-square inputs the row/column indexing is wrong
// and the program either reads the wrong number of characters per row or
// scans the grid transposed.

#include <bits/stdc++.h>
using namespace std;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	const int OBS = -2, WAR = -3, EMP = -1;
	static const int DR[8] = {-1,-1,-1,0,0,1,1,1};
	static const int DC[8] = {-1,0,1,-1,1,-1,0,1};

	int m, n; // BUG: swapped order
	cin >> m >> n;
	vector<string> g(n);
	for (int i = 0; i < n; i++) cin >> g[i];

	vector<pair<int,int>> pos;
	for (int i = 0; i < n; i++)
		for (int j = 0; j < (int)g[i].size(); j++)
			if (g[i][j] == 'S') pos.emplace_back(i, j);
	int k = (int)pos.size();

	int mm = m;
	if ((int)g[0].size() < mm) mm = (int)g[0].size();
	vector<vector<int>> own(n, vector<int>(mm, EMP));
	for (int i = 0; i < n; i++)
		for (int j = 0; j < mm; j++)
			if (g[i][j] == '#') own[i][j] = OBS;
	for (int s = 0; s < k; s++) own[pos[s].first][pos[s].second] = s;

	while (true) {
		vector<tuple<int,int,int>> changes;
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < mm; j++) {
				if (own[i][j] != EMP) continue;
				int first = -1;
				bool war = false;
				for (int d = 0; d < 8; d++) {
					int ni = i + DR[d], nj = j + DC[d];
					if (ni < 0 || ni >= n || nj < 0 || nj >= mm) continue;
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
		for (int j = 0; j < mm; j++) {
			if (own[i][j] == WAR) war++;
			else if (own[i][j] >= 0) cnt[own[i][j]]++;
		}

	cout << war << "\n";
	for (int s = 0; s < k; s++) cout << cnt[s] << "\n";
	return 0;
}
