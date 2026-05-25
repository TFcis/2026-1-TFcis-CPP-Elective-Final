#include <bits/stdc++.h>
#include "testlib.h"
using namespace std;

// Generator for pD.
//
// All inputs are guaranteed valid: T is constructed so that count(c, T) <= count(c, S)
// for every char c. Most modes build T by drawing a random multi-subset of S's bag.
//
// Modes (all argv positions after argv[0]):
//
//   rand_uniform   <n> <m> <lo> <hi> <seed>
//     S uniform random over ASCII [lo, hi]; T = random subset of S, then shuffled.
//
//   rand_k         <n> <m> <k> <seed>
//     Pick k distinct chars at random from [33, 126]; S uniform over them; T = random subset.
//
//   sorted_uniform <n> <m> <lo> <hi> <seed>
//     Like rand_uniform, then S and T are sorted (non-decreasing) -- for sub3.
//
//   sorted_k       <n> <m> <k> <seed>
//     Like rand_k, then sorted.
//
//   single         <n> <m> <c> <flo> <fhi> <seed>
//     T = m copies of char c (ASCII value). S contains EXACTLY m copies of c plus
//     (n - m) "filler" chars drawn from [flo, fhi] excluding c. Order shuffled.
//     For sub1.
//
//   pair           <n> <kind> <c1> <c2> <alpha_lo> <alpha_hi> <seed>
//     m = 2 (sub2). kind=0 forces c1==c2 (T duplicate). kind=1 forces c1!=c2.
//     S uniform over [alpha_lo, alpha_hi]; we guarantee S has enough of T's chars.
//
//   explicit2      <n> <m> <sorted_flag> <c1> <c2> <seed>
//   explicit3      <n> <m> <sorted_flag> <c1> <c2> <c3> <seed>
//     S over the explicit alphabet (so we can hit extreme ASCII pairs like 33,126).
//     sorted_flag = 0 random, 1 sorted (non-decreasing).
//
//   all94          <n> <m> <sorted_flag> <seed>
//     S uses all 94 chars (uniform over [33, 126]); T random subset (or sorted).
//
//   worst          <n> <m> <k> <seed>
//     Adversarial layout: S is k contiguous blocks (each of one char, all distinct);
//     T is one char from each block evenly. Optimal window must span all blocks ->
//     answer is close to n, and "reset r" approaches do O(n^2) work.
//
//   adv_sub2       <n> <kind> <c1> <c2> <seed>
//     Deterministic adversarial structures for sub2 (m = 2 enforced). T = c1c2
//     (with c1 != c2). The five kinds force O(n^2)-style inner walks against
//     the reset-r / early-break / prefilter family even though m = 2:
//       kind=1: A^(n/2)   + B + A^(n/2 - 1)        -- lone B in the middle.
//               Forward AND reverse early-break both pay O(n^2/8).
//       kind=2: A^(n-1)   + B                      -- lone B at the end.
//               Forward early-break pays O(n^2/2); reverse collapses to O(n).
//       kind=3: A         + B^(n-1)                -- lone A at the start.
//               Forward collapses to O(n); reverse early-break pays O(n^2/2).
//       kind=4: (A^3000 + B)^k + (A^t + B)         -- 3000 < t < 6000, k max
//               so the chunked layout fills n. Targets reset-r-without-break:
//               O((n/3001) * 3000^2) ~ n*3000 work total.
//       kind=5: (A^t + B) + (A^3000 + B)^k         -- mirror of kind=4.
//     S is built from only c1 and c2, so prefilter cannot strip anything (np=n).
//
//   decoy          <n> <m> <pos_kind> <seed>
//     Builds S so that the ONLY valid window has length exactly m, placed at:
//       pos_kind = 0  -> start  (S[0 .. m-1])
//       pos_kind = 1  -> middle (S[(n-m)/2 .. (n-m)/2 + m - 1])
//       pos_kind = 2  -> end    (S[n-m .. n-1])
//     T is m distinct chars (so m <= 93 -- need at least 1 filler char). Every
//     other position is the same filler char (not in T). This is the canonical
//     "reset r + break early" worst case: best collapses to m only after l has
//     marched past the window, so before that point inner work per l is huge.
//     - pos_kind=2: forward early-break = O(n^2); reverse(S) makes it O(n).
//     - pos_kind=0: reverse(S) early-break = O(n^2); forward = O(n).
//     - pos_kind=1: both forward and reverse early-break = O(n^2).

static void writeIO(int n, int m, const string& S, const string& T) {
	cout << n << " " << m << "\n";
	cout << S << "\n";
	cout << T << "\n";
}

// Draw m chars from S's bag without replacement -> guarantees solution existence.
// Uses testlib's 2-arg ::shuffle (which drives rnd) -- std::shuffle would need
// a URBG and rnd doesn't satisfy that concept.
static string subsetOf(const string& S, int m) {
	string bag = S;
	::shuffle(bag.begin(), bag.end());
	string T = bag.substr(0, m);
	::shuffle(T.begin(), T.end());
	return T;
}

static vector<char> pickKchars(int k) {
	k = min(k, 94);
	vector<int> all;
	for (int c = 33; c <= 126; c++) all.push_back(c);
	::shuffle(all.begin(), all.end());
	vector<char> out;
	for (int i = 0; i < k; i++) out.push_back((char)all[i]);
	return out;
}

int main(int argc, char* argv[]) {
	registerGen(argc, argv, 1);

	if (argc < 2) {
		cerr << "no mode given" << endl;
		return 1;
	}
	string mode = argv[1];

	if (mode == "rand_uniform") {
		// rand_uniform n m lo hi seed
		int n = atoi(argv[2]);
		int m = atoi(argv[3]);
		int lo = atoi(argv[4]);
		int hi = atoi(argv[5]);
		string S(n, 0);
		for (int i = 0; i < n; i++) S[i] = (char)rnd.next(lo, hi);
		string T = subsetOf(S, m);
		writeIO(n, m, S, T);
	}
	else if (mode == "rand_k") {
		// rand_k n m k seed
		int n = atoi(argv[2]);
		int m = atoi(argv[3]);
		int k = atoi(argv[4]);
		vector<char> chars = pickKchars(k);
		string S(n, 0);
		for (int i = 0; i < n; i++) S[i] = chars[rnd.next((int)chars.size())];
		string T = subsetOf(S, m);
		writeIO(n, m, S, T);
	}
	else if (mode == "sorted_uniform") {
		int n = atoi(argv[2]);
		int m = atoi(argv[3]);
		int lo = atoi(argv[4]);
		int hi = atoi(argv[5]);
		string S(n, 0);
		for (int i = 0; i < n; i++) S[i] = (char)rnd.next(lo, hi);
		sort(S.begin(), S.end());
		string T = subsetOf(S, m);
		sort(T.begin(), T.end());
		writeIO(n, m, S, T);
	}
	else if (mode == "sorted_k") {
		int n = atoi(argv[2]);
		int m = atoi(argv[3]);
		int k = atoi(argv[4]);
		vector<char> chars = pickKchars(k);
		string S(n, 0);
		for (int i = 0; i < n; i++) S[i] = chars[rnd.next((int)chars.size())];
		sort(S.begin(), S.end());
		string T = subsetOf(S, m);
		sort(T.begin(), T.end());
		writeIO(n, m, S, T);
	}
	else if (mode == "single") {
		// single n m c flo fhi seed
		int n = atoi(argv[2]);
		int m = atoi(argv[3]);
		int c = atoi(argv[4]);
		int flo = atoi(argv[5]);
		int fhi = atoi(argv[6]);
		// Ensure filler range has at least one char != c (when n > m).
		if (n > m) {
			bool any = false;
			for (int x = flo; x <= fhi; x++) if (x != c) { any = true; break; }
			if (!any) {
				cerr << "single: filler range [" << flo << "," << fhi
					 << "] has no char != " << c << " but n=" << n << " > m=" << m << endl;
				return 1;
			}
		}
		string T(m, (char)c);
		string S;
		S.reserve(n);
		for (int i = 0; i < m; i++) S += (char)c;
		for (int i = 0; i < n - m; i++) {
			int x;
			do { x = (int)rnd.next(flo, fhi); } while (x == c);
			S += (char)x;
		}
		::shuffle(S.begin(), S.end());
		writeIO(n, m, S, T);
	}
	else if (mode == "pair") {
		// pair n kind c1 c2 alpha_lo alpha_hi seed
		int n = atoi(argv[2]);
		int kind = atoi(argv[3]);
		int c1 = atoi(argv[4]);
		int c2 = atoi(argv[5]);
		int alo = atoi(argv[6]);
		int ahi = atoi(argv[7]);
		if (kind == 0) c2 = c1;
		else if (c1 == c2) {
			cerr << "pair kind=1 but c1==c2" << endl;
			return 1;
		}
		string T;
		T.push_back((char)c1);
		T.push_back((char)c2);
		string S(n, 0);
		for (int i = 0; i < n; i++) S[i] = (char)rnd.next(alo, ahi);
		// Ensure S has enough of c1 and c2.
		int need[256] = {0};
		need[(unsigned char)c1]++;
		need[(unsigned char)c2]++;
		int have[256] = {0};
		for (int i = 0; i < n; i++) have[(unsigned char)S[i]]++;
		for (int c = 0; c < 256; c++) {
			while (have[c] < need[c]) {
				int p = (int)rnd.next(0, n - 1);
				if ((unsigned char)S[p] != c) {
					have[(unsigned char)S[p]]--;
					S[p] = (char)c;
					have[c]++;
				}
			}
		}
		writeIO(n, 2, S, T);
	}
	else if (mode == "explicit2") {
		// explicit2 n m sorted_flag c1 c2 seed
		int n = atoi(argv[2]);
		int m = atoi(argv[3]);
		int sorted_flag = atoi(argv[4]);
		int c1 = atoi(argv[5]);
		int c2 = atoi(argv[6]);
		vector<char> chars = { (char)c1, (char)c2 };
		string S(n, 0);
		for (int i = 0; i < n; i++) S[i] = chars[rnd.next(2)];
		if (sorted_flag) sort(S.begin(), S.end());
		string T = subsetOf(S, m);
		if (sorted_flag) sort(T.begin(), T.end());
		writeIO(n, m, S, T);
	}
	else if (mode == "explicit3") {
		// explicit3 n m sorted_flag c1 c2 c3 seed
		int n = atoi(argv[2]);
		int m = atoi(argv[3]);
		int sorted_flag = atoi(argv[4]);
		int c1 = atoi(argv[5]);
		int c2 = atoi(argv[6]);
		int c3 = atoi(argv[7]);
		vector<char> chars = { (char)c1, (char)c2, (char)c3 };
		string S(n, 0);
		for (int i = 0; i < n; i++) S[i] = chars[rnd.next(3)];
		if (sorted_flag) sort(S.begin(), S.end());
		string T = subsetOf(S, m);
		if (sorted_flag) sort(T.begin(), T.end());
		writeIO(n, m, S, T);
	}
	else if (mode == "all94") {
		// all94 n m sorted_flag seed
		int n = atoi(argv[2]);
		int m = atoi(argv[3]);
		int sorted_flag = atoi(argv[4]);
		string S(n, 0);
		for (int i = 0; i < n; i++) S[i] = (char)rnd.next(33, 126);
		if (sorted_flag) sort(S.begin(), S.end());
		string T = subsetOf(S, m);
		if (sorted_flag) sort(T.begin(), T.end());
		writeIO(n, m, S, T);
	}
	else if (mode == "worst") {
		// worst n m k seed
		// S = k contiguous blocks of distinct chars, sizes as equal as possible.
		// T uses one chunk from each block (m / k each, plus leftovers spread).
		// Optimal window must span all blocks (~n long), so "reset-r" runs O(n^2).
		int n = atoi(argv[2]);
		int m = atoi(argv[3]);
		int k = atoi(argv[4]);
		if (k > 94) k = 94;
		if (k < 1) k = 1;
		vector<char> chars = pickKchars(k);
		// Build S: block i has size roughly n/k, total = n.
		string S;
		S.reserve(n);
		int per = n / k;
		int rem = n - per * k;
		for (int i = 0; i < k; i++) {
			int sz = per + (i < rem ? 1 : 0);
			for (int j = 0; j < sz; j++) S += chars[i];
		}
		// Build T: take ceil(m/k) of first 'rem2' chars, floor(m/k) of rest.
		string T;
		T.reserve(m);
		int per_t = m / k;
		int rem_t = m - per_t * k;
		for (int i = 0; i < k; i++) {
			int sz = per_t + (i < rem_t ? 1 : 0);
			for (int j = 0; j < sz; j++) T += chars[i];
		}
		// Ensure T fits: per_t + 1 (max) must be <= per (min). If not, fall back to
		// drawing T from S.
		bool ok = true;
		{
			int cntS[256] = {0}, cntT[256] = {0};
			for (char c : S) cntS[(unsigned char)c]++;
			for (char c : T) cntT[(unsigned char)c]++;
			for (int c = 0; c < 256; c++) if (cntT[c] > cntS[c]) { ok = false; break; }
		}
		if (!ok) {
			T = subsetOf(S, m);
		}
		writeIO(n, m, S, T);
	}
	else if (mode == "adv_sub2") {
		// adv_sub2 n kind c1 c2 seed
		int n = atoi(argv[2]);
		int kind = atoi(argv[3]);
		int c1 = atoi(argv[4]);
		int c2 = atoi(argv[5]);
		if (c1 == c2) {
			cerr << "adv_sub2: c1 and c2 must differ" << endl;
			return 1;
		}
		string T;
		T.push_back((char)c1);
		T.push_back((char)c2);
		string S;
		if (kind == 1) {
			int half = n / 2;
			S = string(half, (char)c1);
			S.push_back((char)c2);
			S.append(n - half - 1, (char)c1);
		}
		else if (kind == 2) {
			S = string(n - 1, (char)c1);
			S.push_back((char)c2);
		}
		else if (kind == 3) {
			S.push_back((char)c1);
			S.append(n - 1, (char)c2);
		}
		else if (kind == 4 || kind == 5) {
			// (c1^3000 + c2) * k + (c1^t + c2),   with 3000 < t < 6000, k max
			const int CHUNK = 3001;          // 3000 c1's followed by 1 c2
			int k = (n - 3002) / CHUNK;      // ensure t > 3000 and t+1 fits
			int t = n - CHUNK * k - 1;       // length of trailing c1 run
			if (t <= 3000 || t >= 6000) {
				cerr << "adv_sub2 kind=" << kind << ": derived t=" << t
					 << " not in (3000, 6000) for n=" << n << endl;
				return 1;
			}
			auto chunk = [&](int aCount) {
				S.append(aCount, (char)c1);
				S.push_back((char)c2);
			};
			if (kind == 4) {
				for (int i = 0; i < k; i++) chunk(3000);
				chunk(t);
			} else {
				chunk(t);
				for (int i = 0; i < k; i++) chunk(3000);
			}
		}
		else {
			cerr << "adv_sub2: unknown kind " << kind << endl;
			return 1;
		}
		if ((int)S.size() != n) {
			cerr << "adv_sub2: built size " << S.size() << " != n=" << n << endl;
			return 1;
		}
		writeIO(n, 2, S, T);
	}
	else if (mode == "decoy") {
		// decoy n m pos_kind seed
		int n = atoi(argv[2]);
		int m = atoi(argv[3]);
		int pos_kind = atoi(argv[4]);
		if (m > 93) {
			cerr << "decoy: m must be <= 93 (need 1 filler char outside T)" << endl;
			return 1;
		}
		if (m > n) {
			cerr << "decoy: m must be <= n" << endl;
			return 1;
		}
		vector<char> targetChars = pickKchars(m);
		string T(targetChars.begin(), targetChars.end());
		bool used[256] = {false};
		for (int i = 0; i < (int)targetChars.size(); i++) {
			used[(unsigned char)targetChars[i]] = true;
		}
		int filler = -1;
		for (int c = 33; c <= 126; c++) {
			if (!used[c]) { filler = c; break; }
		}
		if (filler == -1) {
			cerr << "decoy: no filler char available" << endl;
			return 1;
		}
		int pos;
		if (pos_kind == 0) pos = 0;
		else if (pos_kind == 1) pos = (n - m) / 2;
		else pos = n - m;
		string S(n, (char)filler);
		for (int i = 0; i < m; i++) S[pos + i] = targetChars[i];
		writeIO(n, m, S, T);
	}
	else {
		cerr << "unknown mode: " << mode << endl;
		return 1;
	}

	return 0;
}
