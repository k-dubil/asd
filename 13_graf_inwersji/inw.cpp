#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>

// Disjoint-set data structure.
class FindUnion {
public:

    // Creates n singletons {0}, ..., {n - 1}.
    explicit FindUnion(unsigned n);

    // Finds and returns the representative of the set to which a belongs.
    unsigned Find(unsigned a);

    // If sets containing a and b are disjoint, merges them.
    void Union(unsigned a, unsigned b);

    // Returns the size of the set containing a.
    unsigned setSize(unsigned a);

    // Returns the number of disjoint sets.
    unsigned numberOfSets() const;

private:

    unsigned n;

    // Vector of parents.
    std::vector<unsigned> p;

    // If i is a representative, d[i] is the number of elements in this set.
    std::vector<unsigned> d;

    unsigned numOfSets;
};

FindUnion::FindUnion(unsigned n) : n(n), numOfSets(n) {
    p.reserve(n);
    for (unsigned i = 0; i < n; ++i) {
        p.push_back(i);
    }

    d = std::vector<unsigned>(n, 1);
}

unsigned FindUnion::Find(unsigned a) {
    unsigned b = a;
    while (b != p[b]) {
        b = p[b];
    }
    // Path compression.
    while (p[a] != b) {
        unsigned c = p[a];
        p[a] = b;
        a = c;
    }
    return b;
}

void FindUnion::Union(unsigned a, unsigned b) {
    a = Find(a), b = Find(b);
    if (a != b) {
        --numOfSets;
        if (d[a] <= d[b]) {
            p[a] = b;
            d[b] += d[a];
        } else {
            p[b] = a;
            d[a] += d[b];
        }
    }
}

unsigned FindUnion::setSize(unsigned a) {
    return d[Find(a)];
}

unsigned FindUnion::numberOfSets() const {
    return numOfSets;
}

using pair = std::pair<unsigned, unsigned>;

bool cmp(pair a, pair b) {
    return a.first < b.first || (a.first == b.first && a.second < b.second);
}

int main() {

    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    unsigned n, a_i;
    std::cin >> n;

    FindUnion sets(n);
    std::stack<unsigned> stack;

    std::cin >> a_i;
    --a_i;
    stack.push(a_i);

    for (unsigned i = 1; i < n; ++i) {
        std::cin >> a_i;
        --a_i;
        unsigned m = a_i;

        while (!stack.empty() && stack.top() > a_i) {
            sets.Union(a_i, stack.top());
            m = std::max(m, stack.top());
            stack.pop();
        }
        stack.push(m);
    }

    std::vector<pair> pairs;
    for (unsigned i = 0; i < n; ++i) {
        pairs.emplace_back(sets.Find(i), i);
    }

    std::sort(pairs.begin(), pairs.end(), cmp);

    std::cout << sets.numberOfSets();

    unsigned prev = n + 1;
    for (auto& e : pairs) {
        if (e.first != prev) {
            std::cout << '\n' << sets.setSize(e.first) << " ";
            prev = e.first;
        }
        std::cout << e.second + 1 << " ";
    }
    std::cout << '\n';

    return 0;
}