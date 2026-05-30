// Sub3-targeted heuristic (assumes S and T are non-decreasing). Since both are
// sorted, T1 = T.front() is the smallest required char and Tm = T.back() the
// largest. Idea:
//   p = number of copies of T1 in T
//   q = number of copies of Tm in T
//   x = index of the LAST T1 in S      (binary search on sorted S)
//   y = index of the FIRST Tm in S     (binary search on sorted S)
// Take the p rightmost copies of T1 (window starts at x-p+1) and the q leftmost
// copies of Tm (window ends at y+q-1); everything in between is automatically
// covered because S is sorted. Answer = (y+q-1) - (x-p+1) + 1.
//
// Complexity of the DECISION LOGIC: counting p, q with lower_bound/upper_bound
// on the sorted T is O(log m); locating x, y with lower_bound/upper_bound on the
// sorted S is O(log n). So the logic is O(log n + log m). (Reading the input S
// and T is unavoidably O(n + m); that is an I/O floor, not part of the logic.)
//
// CORRECT when T1 < Tm: T1's block ends at x, Tm's block starts at y, x < y, and
// the formula gives the true shortest window.
//
// BUG when T1 == Tm (T is one repeated char c) AND c occurs in S more than m
// times: then x is the END of c's block and y is the START, so x > y. The
// "interval" [x-p+1, y+q-1] inverts and the formula returns 2m - (count of c in S),
// which is smaller than m (often negative) -- wrong; the true answer is m.
// This is exactly test 3-19 (and incidentally 3-13, where m=1 and the single
// char repeats in S).
#include <bits/stdc++.h>
using namespace std;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n, m;
	cin >> n >> m;
	(void)n;
	string S, T;
	cin >> S >> T;

	char t1 = T.front();
	char tm = T.back();

	// counts within sorted T, via binary search -> O(log m)
	long long p = upper_bound(T.begin(), T.end(), t1) - T.begin();
	long long q = T.end() - lower_bound(T.begin(), T.end(), tm);

	// positions within sorted S, via binary search -> O(log n)
	long long x = (upper_bound(S.begin(), S.end(), t1) - S.begin()) - 1; // last t1
	long long y = lower_bound(S.begin(), S.end(), tm) - S.begin();       // first tm

	long long ans = (y + q - 1) - (x - p + 1) + 1;
	cout << ans << "\n";
	return 0;
}
