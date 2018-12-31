#include <iostream>
#include <vector>
#include <list>
#include <utility>
#include <tuple>
#include <cmath>
#include <algorithm>

// Returns the largest p, such that 2^p <= k.
static unsigned& largestExponent(unsigned k, unsigned maxK) {
//    assert(0 < k && k <= maxK);

    static std::vector<unsigned> vector;
    static bool initiated = false;

    if (!initiated) {
        initiated = true;

        vector.reserve(maxK);
        for (unsigned p = 0, pow = 1, i = 1; i <= maxK; ++i) {
            if ((pow << 1U) <= i) {
                ++p;
                pow <<= 1U;
            }
            vector.push_back(p);
        }
    }

    return vector[k - 1];
}

int main() {

    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    unsigned n, m, a, b, c, d;
    std::cin >> n >> m;

    std::string s;
    std::cin >> s;


    // Karp-Miller-Rosenberg algorithm
    // dbf = dictionary of basic factors

    std::vector<std::vector<unsigned>> dbf;
    for (unsigned i = 0; i < n; ++i) {
        dbf.emplace_back(1, (unsigned) s[i] - 97);
    }

    for (unsigned k = 1; k <= log2(n); ++k) {
        // tuple(dbf[i][k - 1], dbf[i + 2^{k - 1}][k - 1], i)
        std::list<std::tuple<unsigned, unsigned, unsigned>> list;

        unsigned maxBucketIndex1 = 0, maxBucketIndex2 = 0;

        for (int i = n - (1U << k); i >= 0; --i) {
            maxBucketIndex1 = std::max(maxBucketIndex1, dbf[i][k - 1]);
            maxBucketIndex2 = std::max(maxBucketIndex2, dbf[i + (1U << (k - 1))][k - 1]);

            list.emplace_back(dbf[i][k - 1], dbf[i + (1U << (k - 1))][k - 1], i);
        }
        if (list.empty()) {
            continue;
        }

        // radix sort
        std::vector<std::list<std::tuple<unsigned, unsigned, unsigned>>> buckets;
        for (unsigned i = 0; i < maxBucketIndex2 + 1; ++i) {
            buckets.emplace_back();
        }

        while (!list.empty()) {
            buckets[std::get<1>(*list.begin())].splice(buckets[std::get<1>(*list.begin())].end(), list, list.begin());
        }

        for (auto& bucket : buckets) {
            list.splice(list.end(), bucket);
        }

        if (maxBucketIndex2 < maxBucketIndex1) {
            for (unsigned i = 0; i < maxBucketIndex1 - maxBucketIndex2; ++i) {
                buckets.emplace_back();
            }
        }

        while (!list.empty()) {
            buckets[std::get<0>(*list.begin())].splice(buckets[std::get<0>(*list.begin())].end(), list, list.begin());
        }

        for (auto& bucket : buckets) {
            list.splice(list.end(), bucket);
        }

        unsigned rank = 0;
        auto it = list.begin();

        dbf[std::get<2>(*it)].push_back(rank);
        ++it;

        for (; it != list.end(); ++it) {
            if (std::get<0>(*it) != std::get<0>(*(--it)) || std::get<1>(*(++it)) != std::get<1>(*(--it))) {
                ++rank;
            }
            dbf[std::get<2>(*(++it))].push_back(rank);
        }
    }

    for (unsigned i = 0; i < m; ++i) {
        std::cin >> a >> b >> c >> d;
        --a;
        --b;
        --c;
        --d;

        unsigned len = std::min(b - a + 1, d - c + 1);
        unsigned p = largestExponent(len, n);

        if (dbf[a][p] < dbf[c][p]) {
            std::cout << "<\n";
        } else if (dbf[a][p] > dbf[c][p]) {
            std::cout << ">\n";
        } else {
            if (dbf[a + len - (1U << p)][p] < dbf[c + len - (1U << p)][p]) {
                std::cout << "<\n";
            } else if (dbf[a + len - (1U << p)][p] > dbf[c + len - (1U << p)][p]) {
                std::cout << ">\n";
            } else {
                if (b - a == d - c) {
                    std::cout << "=\n";
                } else if (b - a < d - c) {
                    std::cout << "<\n";
                } else {
                    std::cout << ">\n";
                }
            }
        }
    }

    return 0;
}