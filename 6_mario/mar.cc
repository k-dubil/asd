#include <iostream>
#include <climits>
#include <cmath>
#include <queue>
#include <unordered_map>
#include <unordered_set>

using Vertex = std::pair<unsigned, unsigned>; // pair(index in string, log2(S) - 1)

inline unsigned getI(Vertex vertex) { return vertex.first; }

inline unsigned getS(Vertex vertex) { return vertex.second; }

inline Vertex makeVertex(unsigned i, unsigned S) { return std::make_pair(i, S); }

class Hasher {
public:
    size_t operator()(const Vertex &key) const {
        return std::hash<unsigned>{}(getI(key)) ^ std::hash<unsigned>{}(getS(key));
    }
};

class Cmp {
public:
    bool operator()(const std::pair<Vertex, int> &lhs, const std::pair<Vertex, int> &rhs) const {
        return (lhs.second > rhs.second);
    }
};

int f() {
    unsigned n, K, U;
    std::cin >> n >> K >> U;

    std::string track;
    std::cin >> track;

    if (n <= 2) {
        return n - 1;
    }

    // Dijkstra's algorithm

    std::priority_queue<std::pair<Vertex, int>, std::vector<std::pair<Vertex, int>>, Cmp> queue; // priority_queue(pair(Vertex, distance))
    std::unordered_map<Vertex, int, Hasher> dist; // map Vertex -> distance
    std::unordered_set<Vertex, Hasher> visited; // set(Vertex)

    Vertex start = makeVertex(0, 0);
    queue.push(std::make_pair(start, 0));
    dist.insert(std::make_pair(start, 0));

    while (!queue.empty()) {
        Vertex v = queue.top().first;
        int d = queue.top().second;
        queue.pop();

        if (visited.find(v) != visited.end() && track[getI(v)] != '*') {
            continue;
        }
        visited.insert(v);

        if (getI(v) == n - 1) { // end of the track
            return d;
        }

        switch (track[getI(v)]) {
            case '#' : {
                dist[v] = INT_MAX / 2; // +infinity
            }
                break;
            case '*' : {
                if (log2(n) < getS(v) && track[getI(v) + 1] == '<') { // in case of looping
                    continue;
                }

                Vertex vertex = makeVertex(getI(v), getS(v) + 1);
                visited.insert(vertex);
                dist.insert(std::make_pair(vertex, d));

                Vertex vertex2 = makeVertex(getI(vertex) + 1, getS(vertex));
                dist[vertex2] = std::min(1 + d, dist[vertex2]);
                queue.push(std::make_pair(vertex2, 1 + d));

                if (getS(vertex) + 1 < log2(n) && getI(vertex) + (2ul << getS(vertex)) < n) {
                    vertex2 = makeVertex(getI(vertex) + (2u << getS(vertex)), getS(vertex));
                    dist[vertex2] = std::min(1 + d, dist[vertex2]);
                    queue.push(std::make_pair(vertex2, 1 + d));

                }
            }
                break;
            case '>' : {
                if (getI(v) + K < n) {
                    Vertex vertex = makeVertex(getI(v) + K, getS(v));
                    dist[vertex] = std::min(d, dist[vertex]);
                    queue.push(std::make_pair(vertex, d));
                }
            }
                break;
            case '<' : {
                if (K <= getI(v)) {
                    Vertex vertex = makeVertex(getI(v) - K, getS(v));
                    dist[vertex] = std::min(d, dist[vertex]);
                    queue.push(std::make_pair(vertex, d));
                }
            }
                break;
            case 'U' :
                d += U;
                [[fallthrough]];
            default:
                Vertex vertex = makeVertex(getI(v) + 1, getS(v));
                dist[vertex] = std::min(1 + d, dist[vertex]);
                queue.push(std::make_pair(vertex, 1 + d));

                if (getS(vertex) + 1 < log2(n) && getI(v) + (2ul << getS(v)) < n) {
                    vertex = makeVertex(getI(v) + (2u << getS(v)), getS(v));
                    dist[vertex] = std::min(1 + d, dist[vertex]);
                    queue.push(std::make_pair(vertex, 1 + d));
                }
        }
    }

    return -1;
}

int main() {

    std::cout << f() << std::endl;

    return 0;
}