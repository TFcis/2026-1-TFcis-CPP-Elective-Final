#include <bits/stdc++.h>
#include "testlib.h"
using namespace std;

// pB test data generator.
//
// Usage modes (first argv after the binary name):
//
//   random <n> <s_pool> <a_set> <b_set> <seed_tag>
//       S has length n, each char sampled uniformly from s_pool.
//       A is the chars in a_set; B is the chars in b_set.
//       Use '_' for a_set or b_set to mean empty (printed as "(null)").
//
//   cyclic <t_block> <k> <a_set> <b_set> [seed_tag]
//       S = t_block repeated k times. n = k * |t_block|.
//       Use the literal block as t_block (e.g. "ABCABCA"), or an alias.
//
//   nullabuse <n> <which> <seed_tag>
//       Generates S with a high density of '(', 'n', 'u', 'l', ')' characters,
//       plus other chars. `which` picks which side is (null):
//         A    -> A is empty, B is "abcdefgh"
//         B    -> A is "abcdefgh", B is empty
//         both -> both empty
//       Targets wrong solutions that treat the literal "(null)" string
//       as a character set instead of an empty marker.
//
//   null_s <a_set> <b_set> [seed_tag]
//       Emits S = the literal 6-char string "(null)" (which is itself valid
//       input). Targets wrong solutions that also treat S == "(null)" as
//       "S is empty" -- the spec only marks A/B that way, never S.
//
// Tokens for s_pool / t_block / a_set / b_set:
//   `_`         -> empty set
//   `lower`     -> a..z
//   `upper`     -> A..Z
//   `digit`     -> 0..9
//   `alpha`     -> a..z + A..Z
//   `alnum`     -> 0..9 + a..z + A..Z
//   `ascii`     -> all printable ASCII 33..126
//   anything else -> taken as a literal character list
//
// All trailing args feed testlib's PRNG seed; vary the seed_tag to get distinct
// random outputs from the same mode call.

static string allPrintable() {
	string s;
	for (int c = 33; c <= 126; c++) s += char(c);
	return s;
}

static string resolveAlphabet(const string& id) {
	if (id == "_") return "";
	if (id == "lower") return "abcdefghijklmnopqrstuvwxyz";
	if (id == "upper") return "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	if (id == "digit") return "0123456789";
	if (id == "alpha") return "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	if (id == "alnum") return "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	if (id == "ascii") return allPrintable();
	return id;
}

// Render A or B for output: empty -> "(null)", else literal.
static string renderAB(const string& s) {
	return s.empty() ? string("(null)") : s;
}

static void emitCase(const string& S, const string& A, const string& B) {
	fwrite(S.data(), 1, S.size(), stdout); putchar('\n');
	string AA = renderAB(A);
	fwrite(AA.data(), 1, AA.size(), stdout); putchar('\n');
	string BB = renderAB(B);
	fwrite(BB.data(), 1, BB.size(), stdout); putchar('\n');
}

// Pick a char uniformly from `pool`.
static char pickChar(const string& pool) {
	int idx = (int)rnd.next((long long)0, (long long)pool.size() - 1);
	return pool[idx];
}

int main(int argc, char* argv[]) {
	registerGen(argc, argv, 1);
	// about testlib, see https://codeforces.com/blog/entry/18291

	if (argc < 2) {
		cerr << "Usage: gen <mode> [args...]\n";
		return 1;
	}

	string mode = argv[1];

	if (mode == "random") {
		if (argc < 6) {
			cerr << "random <n> <s_pool> <a_set> <b_set> <seed_tag>\n";
			return 1;
		}
		int n = atoi(argv[2]);
		string pool = resolveAlphabet(argv[3]);
		string A = resolveAlphabet(argv[4]);
		string B = resolveAlphabet(argv[5]);
		ensuref(n >= 1 && n <= 1000000, "random: n out of range, got %d", n);
		ensuref(!pool.empty(), "random: s_pool is empty");
		string S;
		S.resize(n);
		for (int i = 0; i < n; i++) S[i] = pickChar(pool);
		emitCase(S, A, B);
	} else if (mode == "cyclic") {
		if (argc < 6) {
			cerr << "cyclic <t_block> <k> <a_set> <b_set> <seed_tag>\n";
			return 1;
		}
		string T = resolveAlphabet(argv[2]);
		int k = atoi(argv[3]);
		string A = resolveAlphabet(argv[4]);
		string B = resolveAlphabet(argv[5]);
		ensuref(!T.empty() && T.size() <= 8,
			"cyclic: |T| must be in 1..8, got %d", (int)T.size());
		long long total = (long long)k * (long long)T.size();
		ensuref(k >= 1 && total <= 1000000,
			"cyclic: k * |T| must be in 1..1e6, got %lld", total);
		string S;
		S.reserve((size_t)total);
		for (int i = 0; i < k; i++) S += T;
		emitCase(S, A, B);
	} else if (mode == "null_s") {
		if (argc < 4) {
			cerr << "null_s <a_set> <b_set> [seed_tag]\n";
			return 1;
		}
		string A = resolveAlphabet(argv[2]);
		string B = resolveAlphabet(argv[3]);
		emitCase("(null)", A, B);
	} else if (mode == "nullabuse") {
		if (argc < 5) {
			cerr << "nullabuse <n> <which> <seed_tag>\n";
			return 1;
		}
		int n = atoi(argv[2]);
		string which = argv[3];
		ensuref(n >= 1 && n <= 1000000, "nullabuse: n out of range, got %d", n);
		ensuref(which == "A" || which == "B" || which == "both",
			"nullabuse: which must be A/B/both, got %s", which.c_str());
		// Heavy density of the chars in "(null)": '(' 'n' 'u' 'l' ')'
		// plus arbitrary other chars so A/B (when non-empty) have something to act on.
		string pool = "(null)(null)(null)abcdefghQAZ";
		string S; S.resize(n);
		for (int i = 0; i < n; i++) S[i] = pickChar(pool);
		string A = (which == "A" || which == "both") ? string() : string("abcdefgh");
		string B = (which == "B" || which == "both") ? string() : string("QAZ");
		emitCase(S, A, B);
	} else {
		cerr << "Unknown mode: " << mode << "\n";
		return 1;
	}

	return 0;
}
