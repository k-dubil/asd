#include <iostream>
#include <vector>
#include <cmath>
#include <climits>
#include <algorithm>
#include <queue>

using std::min;
using std::abs;

struct Island {

    long x;
    long y;

    unsigned i;

    Island(long _x, long _y, unsigned _i) : x(_x), y(_y), i(_i) {}

};

// Comparators used for sorting vector of islands.
bool cmpX(const Island& lhs, const Island& rhs) {
    return lhs.x < rhs.x || (lhs.x == rhs.x && lhs.y < rhs.y);
}

bool cmpY(const Island& lhs, const Island& rhs) {
    return lhs.y < rhs.y || (lhs.y == rhs.y && lhs.x < rhs.x);
}

// Comparator used by the priority queue.
class CmpPQ {
public:
    bool operator()(const std::pair<Island, long>& lhs, const std::pair<Island, long>& rhs) const {
        return (lhs.second > rhs.second);
    }
};

// Vertex of the graph.
struct Vertex {

    Island island;

    std::vector<Island> neighbors;

    explicit Vertex(Island _island) : island(_island) {}

};

long f() {
    unsigned n;
    long xi, yi;

    std::cin >> n;

    std::vector<Island> islands;
    std::vector<Vertex> graph;

    std::cin >> xi >> yi;
    Island start{xi, yi, 0};
    islands.push_back(start);
    graph.emplace_back(start);

    for (unsigned i = 1; i < n; ++i) {
        std::cin >> xi >> yi;

        Island island{xi, yi, i};

        islands.push_back(island);
        graph.emplace_back(island);
    }

    if (n == 2) {
        return min(abs(islands[0].x - islands[1].x), abs(islands[0].y - islands[1].y));
    }

    // Sort by the coordinate x.
    std::sort(islands.begin(), islands.end(), cmpX);

    for (unsigned i = 0; i < n - 1; ++i) {
        graph[islands[i].i].neighbors.push_back(islands[i + 1]);
        graph[islands[i + 1].i].neighbors.push_back(islands[i]);
    }

    // Sort by the coordinate y.
    std::sort(islands.begin(), islands.end(), cmpY);

    for (unsigned i = 0; i < n - 1; ++i) {
        graph[islands[i].i].neighbors.push_back(islands[i + 1]);
        graph[islands[i + 1].i].neighbors.push_back(islands[i]);
    }

    // Dijkstra's algorithm

    // priority_queue(pair(island, distance))
    std::priority_queue<std::pair<Island, long>, std::vector<std::pair<Island, long>>, CmpPQ> queue;
    std::vector<long> dist(n, LONG_MAX);
    std::vector<bool> visited(n, false);

    dist[start.i] = 0;
    queue.push(std::make_pair(start, dist[start.i]));

    while (!queue.empty()) {
        Island island = queue.top().first;
        long d = queue.top().second;
        queue.pop();

        if (visited[island.i]) {
            continue;
        }
        visited[island.i] = true;

        if (island.i == n - 1) {
            return d; // Last island, end of the cruise.
        }

        for (auto& neighbor : graph[island.i].neighbors) {
            if (!visited[neighbor.i]) {
                dist[neighbor.i] = min(dist[neighbor.i],
                                       d + min(abs(island.x - neighbor.x), abs(island.y - neighbor.y)));
                queue.push(std::make_pair(neighbor, dist[neighbor.i]));
            }
        }
    }

    return -1;
}

int main() {

    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::cout << f() << std::endl;

    return 0;
}