#include <iostream>

void f() {
    static const int MOD = 1000000000;
    int n, k;

    std::cin >> n >> k;

    int a[n];
    int t[n][k];

    for (int i = 0; i < n; ++i) {
        std::cin >> a[i];
    }

//    dynamic programming:
//    1 <= i <= n, 1 <= j <= k

//    t(i, j) = number of j-inversions starting at index i
//    the result is (\sum_{i = 1}^{n} t(i, k)) \mod 10^9
//
//    t(i, 1) = 1
//    t(i, j) = (\sum_{l > i} T(l, j - 1)[a[i] > a[l]]) \mod 10^9

    for (int i = 0; i < n; ++i) {
        t[i][0] = 1;
    }
    for (int j = 1; j < k; ++j) {
        for (int i = n - 1; i >= 0; --i) {
            t[i][j] = 0;
            for (int l = i + 1; l < n; ++l) {
                t[i][j] += t[l][j - 1] * (a[i] > a[l] ? 1 : 0);
                t[i][j] %= MOD;
            }
        }
    }

    int res = 0;
    for (int i = 0; i < n; ++i) {
        res += t[i][k - 1];
        res %= MOD;
    }

    std::cout << res << std::endl;
}

int main() {

    f();

    return 0;
}



