#include <iostream>
#include <queue>
#include <vector>
#include <unordered_set>

class Vertex {

public:

    unsigned value;                             // number of employees
    unsigned outDegree;
    std::unordered_set<unsigned> dependencies;  // dependencies.size() == inDegree

    Vertex() = delete;

    explicit Vertex(unsigned _value, unsigned _outDegree = 0) : value(_value), outDegree(_outDegree) {}

};

class Cmp {
public:
    bool operator()(const Vertex& lhs, const Vertex& rhs) const {
        return (lhs.value > rhs.value);
    }
};

void f() {
    unsigned n, m, k, pi, a, b;
    std::cin >> n >> m >> k;

    std::vector<Vertex> vertices;

    for (unsigned i = 0; i < n; ++i) {
        std::cin >> pi;
        vertices.emplace_back(pi);
    }

    for (unsigned i = 0; i < m; ++i) {
        std::cin >> a >> b;

        vertices[b - 1].dependencies.insert(a - 1);     // edge a -> b
        vertices[a - 1].outDegree++;
    }

    std::priority_queue<Vertex, std::vector<Vertex>, Cmp> queue;
    for (auto& v : vertices) {
        if (v.outDegree == 0) {
            queue.push(v);
        }
    }

    unsigned result = 0, done = 0;
    while (done < k) {
        auto best = queue.top();    // vertex with the smallest value
        queue.pop();

        for (auto& dep : best.dependencies) {
            vertices[dep].outDegree--;
            if (vertices[dep].outDegree == 0) {
                queue.push(vertices[dep]);
            }
        }

        result = std::max(result, best.value);
        ++done;
    }

    std::cout << result << '\n';
}

int main() {

    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    f();

    return 0;
}