#include "testlib.h"
#include <string>
#include <vector>
#include <queue>
#include <set>
using namespace std;

const int MAXN = 100;
const int MAXM = 100;
const int MAXK = 100;

// Cell owner encoding for simulation:
//   -2 = obstacle ('#')
//   -3 = war zone
//   -1 = empty ('.') and not yet claimed
//   >=0 = slime id
static const int OBS = -2;
static const int WAR = -3;
static const int EMP = -1;

static const int DR[8] = {-1,-1,-1, 0, 0, 1, 1, 1};
static const int DC[8] = {-1, 0, 1,-1, 1,-1, 0, 1};

int main(int argc, char* argv[]) {
	registerValidation(argc, argv);

	// Read subtask name from positional argv (testlib consumes its own flags).
	string subtask;
	for (int i = 1; i < argc; i++) {
		string a = argv[i];
		if (a.empty()) continue;
		if (a == "--testset" || a == "--group" || a == "--testOverviewLogFileName") {
			i++;
			continue;
		}
		if (a[0] == '-') continue;
		subtask = a;
		break;
	}

	int n = inf.readInt(1, MAXN, "n");
	inf.readSpace();
	int m = inf.readInt(1, MAXM, "m");
	inf.readEoln();

	vector<string> grid(n, string(m, '.'));
	int k = 0;
	vector<pair<int,int> > slimePos;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			char c = inf.readChar();
			ensuref(c == '.' || c == '#' || c == 'S',
				"row %d col %d: expected '.', '#' or 'S', got 0x%02X",
				i, j, (unsigned char)c);
			grid[i][j] = c;
			if (c == 'S') {
				k++;
				slimePos.push_back(make_pair(i, j));
			}
		}
		inf.readEoln();
	}
	inf.readEof();

	ensuref(k >= 1, "k must be at least 1, got 0");
	ensuref(k <= MAXK, "k must be at most %d, got %d", MAXK, k);

	// Per-subtask special constraints.
	if (subtask == "subtask1") {
		ensuref(k == 1, "subtask1 requires k = 1, got %d", k);
	} else if (subtask == "subtask2") {
		ensuref(n == 1, "subtask2 requires n = 1, got %d", n);
		for (int j = 0; j < m; j++) {
			ensuref(grid[0][j] != '#',
				"subtask2 forbids obstacles, but col %d is '#'", j);
		}
	} else if (subtask == "subtask3") {
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < m; j++) {
				ensuref(grid[i][j] != '#',
					"subtask3 forbids obstacles, but (%d,%d) is '#'", i, j);
			}
		}
	} else if (subtask == "subtask4") {
		// Simulate expansion (frontier-based BFS by rounds) and assert:
		//   (a) no war zone ever forms;
		//   (b) at stable state, no two cells of different slimes are 8-adjacent.
		vector<vector<int> > own(n, vector<int>(m, EMP));
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < m; j++) {
				if (grid[i][j] == '#') own[i][j] = OBS;
			}
		}
		vector<vector<pair<int,int> > > frontier(k);
		for (int s = 0; s < k; s++) {
			int r = slimePos[s].first, c = slimePos[s].second;
			own[r][c] = s;
			frontier[s].push_back(make_pair(r, c));
		}

		while (true) {
			// Map each target empty cell -> set of source slime ids contributing this round.
			// Encode target as r*MAXM + c (m<=MAXM=100, safe).
			vector<vector<int> > srcs(n * (MAXM + 1));
			bool anyTarget = false;
			for (int s = 0; s < k; s++) {
				for (size_t t = 0; t < frontier[s].size(); t++) {
					int r = frontier[s][t].first;
					int c = frontier[s][t].second;
					for (int d = 0; d < 8; d++) {
						int nr = r + DR[d], nc = c + DC[d];
						if (nr < 0 || nr >= n || nc < 0 || nc >= m) continue;
						if (own[nr][nc] != EMP) continue;
						int key = nr * (MAXM + 1) + nc;
						vector<int>& v = srcs[key];
						if (v.empty() || v.back() != s) v.push_back(s);
						anyTarget = true;
					}
				}
			}
			if (!anyTarget) break;
			vector<vector<pair<int,int> > > newFrontier(k);
			for (int r = 0; r < n; r++) {
				for (int c = 0; c < m; c++) {
					int key = r * (MAXM + 1) + c;
					if (srcs[key].empty()) continue;
					// Deduplicate (each slime can appear multiple times if many of its
					// cells border the target).
					set<int> uniq(srcs[key].begin(), srcs[key].end());
					if (uniq.size() == 1) {
						int s = *uniq.begin();
						own[r][c] = s;
						newFrontier[s].push_back(make_pair(r, c));
					} else {
						// War zone formed -> subtask4 violation.
						ensuref(false,
							"subtask4: war zone formed at (%d,%d); slimes are not isolated",
							r, c);
					}
				}
			}
			frontier.swap(newFrontier);
		}

		// Stable state: check no slime cell is 8-adjacent to a different slime's cell.
		for (int r = 0; r < n; r++) {
			for (int c = 0; c < m; c++) {
				if (own[r][c] < 0) continue;
				int a = own[r][c];
				for (int d = 0; d < 8; d++) {
					int nr = r + DR[d], nc = c + DC[d];
					if (nr < 0 || nr >= n || nc < 0 || nc >= m) continue;
					if (own[nr][nc] < 0) continue;
					if (own[nr][nc] != a) {
						ensuref(false,
							"subtask4: cell (%d,%d) [slime %d] is 8-adjacent to (%d,%d) [slime %d]; slimes are not isolated",
							r, c, a, nr, nc, own[nr][nc]);
					}
				}
			}
		}
	}
	// "samples", "full", and unrecognized subtasks: only basic format checks apply.

	return 0;
}
