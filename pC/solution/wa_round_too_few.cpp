#include <bits/stdc++.h>
using namespace std;
#define ll long long
#pragma GCC optimize("Ofast")
const int dx[] = {-1, 0, 1, -1, 1, -1, 0, 1};
const int dy[] = {-1, -1, -1, 0, 0, 1, 1, 1};
signed main() {
    ios::sync_with_stdio(false), cin.tie(0);
    int n, m; cin >> n >> m;
    vector<string> v(n);
    for(auto &d : v) cin >> d;
    vector<vector<int> > belong(n, vector<int> (m)), war(n, vector<int> (m));
    int N = 0;
    for(int i = 0; i < n; i++) for(int j = 0; j < m; j++) if(v[i][j] == 'S')
        belong[i][j] = ++N;
    for(int t = 0; t < n+m; t++) {
        vector<vector<int> > nxt(n, vector<int> (m));
        for(int i = 0; i < n; i++) {
            for(int j = 0; j < m; j++) if(belong[i][j]) {
                for(int k = 0; k < 8; k++) {
                    int xx = i + dx[k], yy = j + dy[k];
                    if(xx < 0 || xx >= n || yy < 0 || yy >= m) continue;
                    if(belong[xx][yy] != 0 || war[xx][yy]) continue;
                    if(v[xx][yy] == '#') continue;
                    if(nxt[xx][yy] != 0 && nxt[xx][yy] != belong[i][j]) {
                        war[xx][yy] = 1;
                        nxt[xx][yy] = 0;
                    } else nxt[xx][yy] = belong[i][j];
                }
            }
        }
        for(int i = 0; i < n; i++) for(int j = 0; j < m; j++) belong[i][j] |= nxt[i][j];
    }
    vector<int> ans(N+1);
    int Ans = 0;
    for(int i = 0; i < n; i++) for(int j = 0; j < m; j++) ans[belong[i][j]]++, Ans += war[i][j];
    cout << Ans << '\n';
    for(int i = 1; i <= N; i++) cout << ans[i] << '\n';
}
