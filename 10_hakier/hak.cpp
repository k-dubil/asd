#include <iostream>
#include <sstream>
#include <vector>
#include <set>

static const unsigned M = 131072; // 2^17, the smallest power of 2 not less than 10^5.

// Interval `[begin, end]`.
struct Interval {

    unsigned begin;
    unsigned end;
    unsigned index;

    Interval(unsigned b, unsigned e, unsigned i) : begin(b), end(e), index(i) {}

    bool operator<(const Interval& rhs) const {
        return begin < rhs.begin || (begin == rhs.begin && end < rhs.end)
               || (begin == rhs.begin && end == rhs.end && index < rhs.index);
    }

};

// Node of the interval tree.
struct Node {

    unsigned begin;     // Beginning of the range corresponding to the Node.

    int minEnd;         // The smallest end of the interval that begins in the range corresponding to the Node.
    int minEndIndex;    // Index of the interval with the smallest end.

    int maxEnd;         // The biggest end of the interval that begins in the range corresponding to the Node.
    int maxEndIndex;    // Index of the interval with the biggest end.

    explicit Node(unsigned b, int minE = -1, int minEi = -1, int maxE = -1, int maxEi = -1)
            : begin(b), minEnd(minE), minEndIndex(minEi), maxEnd(maxE), maxEndIndex(maxEi) {}

};

class IntervalTree {
public:

    IntervalTree();

    void insertInterval(Interval interval);

    // Returns `pair(minEndIndex, minEnd)` of the intervals that begins in range `[begin, end]`.
    std::pair<int, int> queryMin(unsigned begin, unsigned end) const;

    // Returns `pair(maxEndIndex, maxEnd)` of the intervals that begins in range `[begin, end]`.
    std::pair<int, int> queryMax(unsigned begin, unsigned end) const;

    // Removes interval with the smallest end that begins exactly at `begin`.
    void removeMin(unsigned begin);

    // Removes interval with the biggest end that begins exactly at `begin`.
    void removeMax(unsigned begin);

private:

    // Initiates the tree.
    void init();

    // Vector of nodes, v.size() == 2 * M - 1 after init().
    std::vector<Node> v;

    // Vector of intervals beginning at specific points.
    std::vector<std::set<Interval>> intervals{M};

    // Corrects the structure after removing an interval.
    void correct(unsigned index, unsigned removed);

};

IntervalTree::IntervalTree() {
    init();
}

void IntervalTree::init() {
    v.reserve(2 * M - 1);

    // leaves
    for (unsigned i = M - 1; i < 2 * M - 1; ++i) {
        v[i] = Node(i - (M - 1));
    }

    // internal nodes
    for (unsigned i = M - 1; i >= 1; --i) {
        v[i - 1] = Node(v[2 * i - 1]);
    }
}

void IntervalTree::correct(unsigned index, unsigned removed) {
    unsigned i = index / 2;

    while (i >= 1) {
        if ((unsigned) v[i - 1].minEndIndex == removed) {
            v[i - 1].minEnd = v[2 * i - 1].minEnd;
            v[i - 1].minEndIndex = v[2 * i - 1].minEndIndex;

            if (v[2 * i].minEnd != -1 &&
                (v[i - 1].minEnd == -1 || v[i - 1].minEnd > v[2 * i].minEnd ||
                 (v[i - 1].minEnd == v[2 * i].minEnd && v[i - 1].minEndIndex > v[2 * i].minEndIndex))) {
                v[i - 1].minEnd = v[2 * i].minEnd;
                v[i - 1].minEndIndex = v[2 * i].minEndIndex;
            }
        }
        if ((unsigned) v[i - 1].maxEndIndex == removed) {
            v[i - 1].maxEnd = v[2 * i - 1].maxEnd;
            v[i - 1].maxEndIndex = v[2 * i - 1].maxEndIndex;

            if (v[2 * i].maxEnd != -1 &&
                (v[i - 1].maxEnd == -1 || v[i - 1].maxEnd < v[2 * i].maxEnd ||
                 (v[i - 1].maxEnd == v[2 * i].maxEnd && v[i - 1].maxEndIndex < v[2 * i].maxEndIndex))) {
                v[i - 1].maxEnd = v[2 * i].maxEnd;
                v[i - 1].maxEndIndex = v[2 * i].maxEndIndex;
            }
        }

        i /= 2;
    }
}

void IntervalTree::insertInterval(Interval interval) {
    intervals.at(interval.begin).insert(interval);

    unsigned i = interval.begin + M;
    if (v[i - 1].minEnd == -1) {
        v[i - 1].minEnd = interval.end;
        v[i - 1].minEndIndex = interval.index;

        v[i - 1].maxEnd = interval.end;
        v[i - 1].maxEndIndex = interval.index;
    } else {
        if (interval.end < (unsigned) v[i - 1].minEnd ||
            (interval.end == (unsigned) v[i - 1].minEnd && interval.index < (unsigned) v[i - 1].minEndIndex)) {
            v[i - 1].minEnd = interval.end;
            v[i - 1].minEndIndex = interval.index;
        } else if ((unsigned) v[i - 1].maxEnd < interval.end ||
                   ((unsigned) v[i - 1].maxEnd == interval.end && (unsigned) v[i - 1].maxEndIndex < interval.index)) {
            v[i - 1].maxEnd = interval.end;
            v[i - 1].maxEndIndex = interval.index;
        }
    }

    unsigned j = i - 1;
    i /= 2;

    while (i >= 1) {
        --i;
        if (v[i].minEnd == -1) {
            v[i].minEnd = v[j].minEnd;
            v[i].minEndIndex = v[j].minEndIndex;

            v[i].maxEnd = v[j].maxEnd;
            v[i].maxEndIndex = v[j].maxEndIndex;
        } else {
            if (v[i].minEnd > v[j].minEnd ||
                (v[i].minEnd == v[j].minEnd && v[i].minEndIndex > v[j].minEndIndex)) {
                v[i].minEnd = v[j].minEnd;
                v[i].minEndIndex = v[j].minEndIndex;
            }
            if (v[i].maxEnd < v[j].maxEnd ||
                (v[i].maxEnd == v[j].maxEnd && v[i].maxEndIndex < v[j].maxEndIndex)) {
                v[i].maxEnd = v[j].maxEnd;
                v[i].maxEndIndex = v[j].maxEndIndex;
            }
        }
        j = i;
        ++i;

        i /= 2;
    }
}

std::pair<int, int> IntervalTree::queryMin(unsigned begin, unsigned end) const {
    int v1 = M + begin, v2 = M + end;

    int res = v[v1 - 1].minEnd;
    int resIndex = v[v1 - 1].minEndIndex;
    if (v1 != v2) {
        if (res == -1) {
            res = v[v2 - 1].minEnd;
            resIndex = v[v2 - 1].minEndIndex;
        } else {
            if (v[v2 - 1].minEnd != -1 &&
                (res > v[v2 - 1].minEnd || (res == v[v2 - 1].minEnd && resIndex > v[v2 - 1].minEndIndex))) {
                res = v[v2 - 1].minEnd;
                resIndex = v[v2 - 1].minEndIndex;
            }
        }
    }

    while (v1 / 2 != v2 / 2) {
        if (v1 % 2 == 0) {
            if (res == -1) {
                res = v[v1].minEnd;
                resIndex = v[v1].minEndIndex;
            } else {
                if (v[v1].minEnd != -1 &&
                    (res > v[v1].minEnd || (res == v[v1].minEnd && resIndex > v[v1].minEndIndex))) {
                    res = v[v1].minEnd;
                    resIndex = v[v1].minEndIndex;
                }
            }
        }
        if (v2 % 2 == 1) {
            if (res == -1) {
                res = v[v2 - 2].minEnd;
                resIndex = v[v2 - 2].minEndIndex;
            } else {
                if (v[v2 - 2].minEnd != -1 &&
                    (res > v[v2 - 2].minEnd || (res == v[v2 - 2].minEnd && resIndex > v[v2 - 2].minEndIndex))) {
                    res = v[v2 - 2].minEnd;
                    resIndex = v[v2 - 2].minEndIndex;
                }
            }
        }
        v1 /= 2;
        v2 /= 2;
    }

    return std::make_pair(resIndex, res);
}

std::pair<int, int> IntervalTree::queryMax(unsigned begin, unsigned end) const {
    int v1 = M + begin, v2 = M + end;

    int res = v[v1 - 1].maxEnd;
    int resIndex = v[v1 - 1].maxEndIndex;
    if (v1 != v2) {
        if (res == -1) {
            res = v[v2 - 1].maxEnd;
            resIndex = v[v2 - 1].maxEndIndex;
        } else {
            if (v[v2 - 1].maxEnd != -1 &&
                (res < v[v2 - 1].maxEnd || (res == v[v2 - 1].maxEnd && resIndex < v[v2 - 1].maxEndIndex))) {
                res = v[v2 - 1].maxEnd;
                resIndex = v[v2 - 1].maxEndIndex;
            }
        }
    }

    while (v1 / 2 != v2 / 2) {
        if (v1 % 2 == 0) {
            if (res == -1) {
                res = v[v1].maxEnd;
                resIndex = v[v1].maxEndIndex;
            } else {
                if (v[v1].maxEnd != -1 &&
                    (res < v[v1].maxEnd || (res == v[v1].maxEnd && resIndex < v[v1].maxEndIndex))) {
                    res = v[v1].maxEnd;
                    resIndex = v[v1].maxEndIndex;
                }
            }
        }
        if (v2 % 2 == 1) {
            if (res == -1) {
                res = v[v2 - 2].maxEnd;
                resIndex = v[v2 - 2].maxEndIndex;
            } else {
                if (v[v2 - 2].maxEnd != -1 &&
                    (res < v[v2 - 2].maxEnd || (res == v[v2 - 2].maxEnd && resIndex < v[v2 - 2].maxEndIndex))) {
                    res = v[v2 - 2].maxEnd;
                    resIndex = v[v2 - 2].maxEndIndex;
                }
            }
        }
        v1 /= 2;
        v2 /= 2;
    }

    return std::make_pair(resIndex, res);
}

void IntervalTree::removeMin(unsigned begin) {
    auto it = intervals[begin].begin();
    unsigned removed = it->index;
    intervals[begin].erase(it);

    unsigned i = begin + M;
    if (intervals[begin].empty()) {
        v[i - 1].minEnd = -1;
        v[i - 1].minEndIndex = -1;

        v[i - 1].maxEnd = -1;
        v[i - 1].maxEndIndex = -1;
    } else {
        it = intervals[begin].begin();

        v[i - 1].minEnd = it->end;
        v[i - 1].minEndIndex = it->index;
    }

    correct(i, removed);
}

void IntervalTree::removeMax(unsigned begin) {
    auto it = intervals[begin].end();
    --it;
    unsigned removed = it->index;
    intervals[begin].erase(it);

    unsigned i = begin + M;
    if (intervals[begin].empty()) {
        v[i - 1].minEnd = -1;
        v[i - 1].minEndIndex = -1;

        v[i - 1].maxEnd = -1;
        v[i - 1].maxEndIndex = -1;
    } else {
        it = intervals[begin].end();
        --it;

        v[i - 1].maxEnd = it->end;
        v[i - 1].maxEndIndex = it->index;
    }

    correct(i, removed);
}

void f() {

    unsigned n, ai, bi, q, xi, yi;
    std::string ti;

    std::cin >> n;

    IntervalTree tree;

    std::vector<Interval> intervals;
    intervals.reserve(n);

    for (unsigned i = 0; i < n; ++i) {
        std::cin >> ai >> bi;

        tree.insertInterval(Interval{ai - 1, bi - 1, i});
        intervals.emplace_back(ai - 1, bi - 1, i);
    }

    std::cin >> q;

    std::ostringstream s;

    for (unsigned i = 0; i < q; ++i) {
        std::cin >> ti >> xi >> yi;

        s << " ";

        std::pair<int, int> queryRes;

        if (ti == "in") {
            queryRes = tree.queryMin(xi - 1, yi - 1);

            if (queryRes.first != -1 && (unsigned) queryRes.second <= yi - 1) {
                tree.removeMin(intervals.at((unsigned) queryRes.first).begin);
                s << queryRes.first + 1;
            } else {
                s << -1;
            }
        } else if (ti == "over") {
            queryRes = tree.queryMax(0, xi - 1);

            if (queryRes.first != -1 && (unsigned) queryRes.second >= yi - 1) {
                tree.removeMax(intervals.at((unsigned) queryRes.first).begin);
                s << queryRes.first + 1;
            } else {
                s << -1;
            }
        } else if (ti == "none") {
            bool found = false;
            if (1 < xi) {
                queryRes = tree.queryMin(0, xi - 2);

                if (queryRes.first != -1 && (unsigned) queryRes.second < xi - 1) {
                    found = true;
                    tree.removeMin(intervals.at((unsigned) queryRes.first).begin);
                    s << queryRes.first + 1;
                }
            }
            if (!found && yi < 100000) {
                queryRes = tree.queryMin(yi, 100000 - 1);

                if (queryRes.first != -1) {
                    found = true;
                    tree.removeMin(intervals.at((unsigned) queryRes.first).begin);
                    s << queryRes.first + 1;
                }
            }
            if (!found) {
                s << -1;
            }
        } else { // some
            queryRes = tree.queryMax(0, yi - 1);

            if (queryRes.first != -1 && (unsigned) queryRes.second >= xi - 1) {
                tree.removeMax(intervals.at((unsigned) queryRes.first).begin);
                s << queryRes.first + 1;
            } else {
                s << -1;
            }
        }
    }

    std::cout << s.str().erase(0, 1) << '\n';
}

int main() {

    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    f();

    return 0;
}