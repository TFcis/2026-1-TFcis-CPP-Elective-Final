#include <bits/stdc++.h>
#include "testlib.h"
using namespace std;

static void printGrid(int n, int m, const vector<string>& g) {
	cout << n << " " << m << "\n";
	for (int i = 0; i < n; i++) cout << g[i] << "\n";
}

static int placeSlimes(vector<string>& g, int k) {
	int n = (int)g.size();
	int m = (int)g[0].size();
	vector<int> emp;
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			if (g[i][j] == '.') emp.push_back(i * m + j);
	if ((int)emp.size() < k) k = (int)emp.size();
	shuffle(emp.begin(), emp.end());
	for (int t = 0; t < k; t++) {
		int p = emp[t];
		g[p / m][p % m] = 'S';
	}
	return k;
}

int main(int argc, char* argv[]) {
	registerGen(argc, argv, 1);
	// All argv tokens feed the seed; vary any trailing token to get a distinct test.

	string mode = argv[1];

	if (mode == "single") {
		// single <n> <m> <obs_pct> <seed>
		int n = atoi(argv[2]), m = atoi(argv[3]), dens = atoi(argv[4]);
		vector<string> g(n, string(m, '.'));
		for (int i = 0; i < n; i++)
			for (int j = 0; j < m; j++)
				if ((int)rnd.next(0, 99) < dens) g[i][j] = '#';
		int placed = placeSlimes(g, 1);
		if (placed == 0) {
			int r = (int)rnd.next(0, n - 1);
			int c = (int)rnd.next(0, m - 1);
			g[r][c] = 'S';
		}
		printGrid(n, m, g);
	}
	else if (mode == "onerow") {
		// onerow <m> <k> <seed>
		int m = atoi(argv[2]), k = atoi(argv[3]);
		vector<string> g(1, string(m, '.'));
		placeSlimes(g, k);
		printGrid(1, m, g);
	}
	else if (mode == "noobs") {
		// noobs <n> <m> <k> <seed>
		int n = atoi(argv[2]), m = atoi(argv[3]), k = atoi(argv[4]);
		vector<string> g(n, string(m, '.'));
		placeSlimes(g, k);
		printGrid(n, m, g);
	}
	else if (mode == "chambers") {
		// chambers <n> <m> <rr> <cc> <num_slimes> <seed>
		// rr x cc chambers separated by 1-thick obstacle walls.
		int n = atoi(argv[2]), m = atoi(argv[3]);
		int rr = atoi(argv[4]), cc = atoi(argv[5]);
		int num = atoi(argv[6]);
		if (rr < 1) rr = 1;
		if (cc < 1) cc = 1;
		while (rr > 1 && n - (rr - 1) < rr) rr--; // chamber height >= 1
		while (cc > 1 && m - (cc - 1) < cc) cc--;
		vector<int> rs(rr), re_(rr);
		{
			int availR = n - (rr - 1);
			int baseR = availR / rr;
			int extraR = availR % rr;
			int cur = 0;
			for (int i = 0; i < rr; i++) {
				int h = baseR + (i < extraR ? 1 : 0);
				rs[i] = cur;
				re_[i] = cur + h - 1;
				cur += h;
				if (i < rr - 1) cur++;
			}
		}
		vector<int> ks(cc), ke(cc);
		{
			int availC = m - (cc - 1);
			int baseC = availC / cc;
			int extraC = availC % cc;
			int cur = 0;
			for (int j = 0; j < cc; j++) {
				int w = baseC + (j < extraC ? 1 : 0);
				ks[j] = cur;
				ke[j] = cur + w - 1;
				cur += w;
				if (j < cc - 1) cur++;
			}
		}
		vector<string> g(n, string(m, '.'));
		for (int i = 0; i < rr - 1; i++) {
			int wr = re_[i] + 1;
			for (int x = 0; x < m; x++) g[wr][x] = '#';
		}
		for (int j = 0; j < cc - 1; j++) {
			int wc = ke[j] + 1;
			for (int x = 0; x < n; x++) g[x][wc] = '#';
		}
		vector<int> ch_idx;
		for (int i = 0; i < rr; i++)
			for (int j = 0; j < cc; j++)
				ch_idx.push_back(i * cc + j);
		shuffle(ch_idx.begin(), ch_idx.end());
		if (num > (int)ch_idx.size()) num = (int)ch_idx.size();
		for (int t = 0; t < num; t++) {
			int idx = ch_idx[t];
			int ci = idx / cc, cj = idx % cc;
			int r = (int)rnd.next((long long)rs[ci], (long long)re_[ci]);
			int c = (int)rnd.next((long long)ks[cj], (long long)ke[cj]);
			g[r][c] = 'S';
		}
		printGrid(n, m, g);
	}
	else if (mode == "random") {
		// random <n> <m> <obs_pct> <k> <seed>
		int n = atoi(argv[2]), m = atoi(argv[3]), dens = atoi(argv[4]), k = atoi(argv[5]);
		vector<string> g(n, string(m, '.'));
		for (int i = 0; i < n; i++)
			for (int j = 0; j < m; j++)
				if ((int)rnd.next(0, 99) < dens) g[i][j] = '#';
		placeSlimes(g, k);
		printGrid(n, m, g);
	}
	else if (mode == "cluster") {
		// cluster <n> <m> <k> <radius> <seed>
		// k slimes packed within `radius` of map center.
		int n = atoi(argv[2]), m = atoi(argv[3]), k = atoi(argv[4]), radius = atoi(argv[5]);
		vector<string> g(n, string(m, '.'));
		int cr = n / 2, ccol = m / 2;
		vector<pair<int,int> > cand;
		for (int i = max(0, cr - radius); i <= min(n - 1, cr + radius); i++)
			for (int j = max(0, ccol - radius); j <= min(m - 1, ccol + radius); j++)
				cand.push_back(make_pair(i, j));
		shuffle(cand.begin(), cand.end());
		if (k > (int)cand.size()) k = (int)cand.size();
		for (int t = 0; t < k; t++) g[cand[t].first][cand[t].second] = 'S';
		printGrid(n, m, g);
	}
	else if (mode == "bfsbreak") {
		// bfsbreak <variant> <seed>
		// Patterns designed to defeat the "independent BFS per slime" wrong solution
		// (cells that look equally reachable from two slimes via free-space BFS
		// but are actually unreachable due to obstacle walls on the real path).
		int v = atoi(argv[2]);
		if (v == 1) {
			vector<string> g;
			g.push_back("#.S");
			g.push_back("...");
			g.push_back("#.S");
			printGrid(3, 3, g);
		} else if (v == 2) {
			vector<string> g;
			g.push_back("#..S....");
			g.push_back("........");
			g.push_back("#..S....");
			g.push_back("########");
			g.push_back("....S...");
			g.push_back("........");
			g.push_back("....S...");
			printGrid(7, 8, g);
		} else if (v == 3) {
			vector<string> g;
			g.push_back("S.#.....#.S");
			g.push_back("..#.....#..");
			g.push_back("..#######..");
			g.push_back("...........");
			g.push_back("S.#.....#.S");
			printGrid(5, 11, g);
		} else if (v == 4) {
			// Three slimes targeting the same cell — defeats the "third overwrites" WA.
			vector<string> g;
			g.push_back("S.S");
			g.push_back("...");
			g.push_back(".S.");
			printGrid(3, 3, g);
		} else {
			vector<string> g;
			g.push_back(".S.S.");
			g.push_back("#####");
			g.push_back(".S.S.");
			printGrid(3, 5, g);
		}
	}
	else if (mode == "corner") {
		// corner <n> <m> <k> <seed>
		// Up to 4 slimes at corners; the rest placed randomly.
		int n = atoi(argv[2]), m = atoi(argv[3]), k = atoi(argv[4]);
		vector<string> g(n, string(m, '.'));
		vector<pair<int,int> > corners;
		corners.push_back(make_pair(0, 0));
		corners.push_back(make_pair(0, m - 1));
		corners.push_back(make_pair(n - 1, 0));
		corners.push_back(make_pair(n - 1, m - 1));
		shuffle(corners.begin(), corners.end());
		int placed = 0;
		for (size_t i = 0; i < corners.size() && placed < k; i++) {
			int r = corners[i].first, c = corners[i].second;
			if (g[r][c] == '.') { g[r][c] = 'S'; placed++; }
		}
		if (placed < k) placeSlimes(g, k - placed);
		printGrid(n, m, g);
	}
	else if (mode == "adjacent") {
		// adjacent <n> <m> <k> <seed>
		// Slimes placed in a checkerboard so many pairs start 8-adjacent —
		// stress for the "adjacent_war" WA (which would falsely mark them as war zones).
		int n = atoi(argv[2]), m = atoi(argv[3]), k = atoi(argv[4]);
		vector<string> g(n, string(m, '.'));
		vector<pair<int,int> > cand;
		for (int i = 0; i < n; i++)
			for (int j = 0; j < m; j++)
				if (((i + j) & 1) == 0) cand.push_back(make_pair(i, j));
		shuffle(cand.begin(), cand.end());
		if (k > (int)cand.size()) k = (int)cand.size();
		for (int t = 0; t < k; t++) g[cand[t].first][cand[t].second] = 'S';
		printGrid(n, m, g);
	}
	else {
		// Default fallback.
		int n = 5, m = 5;
		vector<string> g(n, string(m, '.'));
		g[0][0] = 'S';
		printGrid(n, m, g);
	}

	return 0;
}
