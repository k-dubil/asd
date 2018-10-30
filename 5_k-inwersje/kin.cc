#include <iostream>
#include <cmath>
#include <vector>
#include <map>

using std::vector;

static const int MOD = 1000000000;

class IntervalTree {
    long n, M;
    vector<long> v; // v.size() == 2 * M - 1

public:
    void init(vector<int> A); // initiates tree

    long query(int a, int b); // A[a] + A[a + 1] + ... + A[b]

    void update(int i, long value); // sets A[i] to value
};

void IntervalTree::init(vector<int> A) {
    n = A.size();
    M = (long) pow(2.0, ceil(log2(double(n)))); // M == closest power of 2 not less than n

    // leaves
    for (int i = 0; i < M - 1; ++i) {
        v.push_back(0);
    }
    for (int e : A) {
        v.push_back(e);
    }
    for (auto i = M + n - 1; i < 2 * M - 1; ++i) {
        v.push_back(0);
    }

    // internal nodes
    for (auto i = (2 * M - 1) / 2 - 1; i >= 0; --i) {
        v[i] = v[2 * (i + 1) - 1] + v[2 * (i + 1)];
        v[i] %= MOD;
    }
}

long IntervalTree::query(int a, int b) {
    long va = M + a - 1, vb = M + b - 1;

    long res = v[va];
    if (va != vb) {
        res += v[vb];
    }
    res %= MOD;

    while ((va + 1) / 2 != (vb + 1) / 2) {
        if (va % 2 == 1) {
            res += v[va + 1];
            res %= MOD;
        }
        if (vb % 2 == 0) {
            res += v[vb - 1];
            res %= MOD;
        }
        va = (va + 1) / 2 - 1;
        vb = (vb + 1) / 2 - 1;
    }
    return res;
}

void IntervalTree::update(int i, long value) {
    v[M + i - 1] = value % MOD;
    for (long k = (M + i) / 2 - 1; k >= 0; k = (k + 1) / 2 - 1) {
        v[k] = v[2 * (k + 1) - 1] + v[2 * (k + 1)];
        v[k] %= MOD;
    }
}


void f() {
    int n, k, a_i;
    std::cin >> n >> k;

    vector<int> a;
    for (int i = 0; i < n; ++i) {
        std::cin >> a_i;
        a.push_back(a_i);
    }

    vector<IntervalTree> v;
    for (int j = 0; j < k; ++j) {
        v.emplace_back(IntervalTree());
        v[j].init(vector<int>(n, 0));
    }
    int idxs[n], idx = 0;
    for (auto e : a) {
        idxs[e - 1] = idx++; // idxs[i] = index of the i-th smallest element of a
    }

    // see the description in file 'kin-naive.cc'
    for (auto i : idxs) {
        v[0].update(i, 1);
        for (int j = 1; j < k; ++j) {
            if (i < n - 1) {
                v[j].update(i, v[j - 1].query(i + 1, n - 1));
            }
        }
    }

    std::cout << v[k - 1].query(0, n - 1) << std::endl;
}

int main() {

    f();

    return 0;
}