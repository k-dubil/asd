#include <iostream>
#include <utility>
#include <vector>
#include <queue>
#include <cmath>

class Vertex {

public:

    Vertex(unsigned i, int left, int right, int parent) : _i(i), _left(left), _right(right), _parent(parent) {};

//    inline unsigned i() const {
//        return _i;
//    }

    inline int left() const {
        return _left;
    }

    inline int right() const {
        return _right;
    }

    inline int parent() const {
        return _parent;
    }

    void setParent(int parent) {
        _parent = parent;
    }

private:
    unsigned _i;
    int _left;
    int _right;
    int _parent;
};


class BinaryTree {

public:

    BinaryTree() = delete;

    BinaryTree(unsigned n, std::vector<Vertex>&& vertices) : _n(n), _vertices(vertices) {}

    inline unsigned size() const {
        return _n;
    }

    inline const Vertex& vertex(int v) const {
        return _vertices[v];
    }

private:

    const unsigned _n; // number of vertices (_n == _vertices.size())

    std::vector<Vertex> _vertices;
};

// Breadth-first search used to calculate the depth of vertices (distance from the root).
void BFS(const BinaryTree& tree, std::vector<unsigned>& dist, unsigned root = 0) {
    std::queue<int> queue;
    queue.push(root);

    while (!queue.empty()) {
        auto v = queue.front();
        queue.pop();

        for (int u : {tree.vertex(v).left(), tree.vertex(v).right()}) {
            if (u != -1) {
                dist[u] = dist[v] + 1;
                queue.push(u);
            }
        }
    }
}

// Computes ancestors at the distances that are powers of two.
void ancestor2(const BinaryTree& tree, std::vector<std::vector<int>>& _ancestor2) {
    unsigned n = tree.size();
    for (unsigned v = 0; v < n; ++v) {
        _ancestor2[v].push_back(tree.vertex(v).parent());
    }
    for (unsigned i = 1; (2u << (i - 1)) <= n; ++i) {
        for (unsigned v = 0; v < n; ++v) {
            if (_ancestor2[v][i - 1] == -1) {
                _ancestor2[v].push_back(-1);
            } else {
                _ancestor2[v].push_back(_ancestor2[_ancestor2[v][i - 1]][i - 1]);
            }
        }
    }
}

// Returns ancestor of the vertex v at the distances equal to d.
unsigned ancestor(const BinaryTree& tree, unsigned v, unsigned d, const std::vector<std::vector<int>>& ancestor2,
                  const std::vector<unsigned>& depth) {
    if (d == 0) {
        return v;
    } else if (depth[v] <= d) {
        return 0;
    }

    int result = v;
    auto i = (unsigned) log2(tree.size());
    unsigned pow = (2u << (i - 1));
    while (d > 0) {
        if (d < pow) {
            --i;
            pow >>= 1;
        } else {
            result = ancestor2[result][i];
            d -= pow;
        }
    }

    return (unsigned) result;
}

// Returns lowest common ancestor of the vertices v and u.
unsigned lca(const BinaryTree& tree, unsigned v, unsigned u, const std::vector<std::vector<int>>& ancestor2,
             const std::vector<unsigned>& depth) {
    if (depth[v] < depth[u]) {
        u = ancestor(tree, u, depth[u] - depth[v], ancestor2, depth);
    } else if (depth[u] < depth[v]) {
        v = ancestor(tree, v, depth[v] - depth[u], ancestor2, depth);
    }

    if (v == u) {
        return v;
    }

    auto i = (int) log2(depth[v]);
    while (i >= 0) {
        int v2 = ancestor2[v][i], u2 = ancestor2[u][i];
        if (v2 != u2) {
            v = (unsigned) v2;
            u = (unsigned) u2;
        }
        --i;
    }

    return (unsigned) tree.vertex(v).parent();
}

// Computes vector of farthest vertices (and max distances) in the subtree v.
void farDown(const BinaryTree& tree, unsigned v, std::vector<std::pair<unsigned, unsigned>>& fd) {
    // post-order traversal
    for (auto u : {tree.vertex(v).left(), tree.vertex(v).right()}) {
        if (u != -1) {
            farDown(tree, (unsigned) u, fd);
        }
    }

    std::pair<unsigned, unsigned> best{v, 0};
    for (auto u : {tree.vertex(v).left(), tree.vertex(v).right()}) {
        if (u != -1 && best.second < fd[u].second + 1) {
            best = std::make_pair(fd[u].first, fd[u].second + 1);
        }
    }

    fd[v] = best;
}

// Computes vector of farthest vertices (and max distances) in the tree with removed descendants of vertex v.
void farUp(const BinaryTree& tree, unsigned v, std::vector<std::pair<unsigned, unsigned>>& fu,
           const std::vector<std::vector<int>>& ancestor2, const std::vector<unsigned>& depth,
           const std::vector<std::pair<unsigned, unsigned>>& fd) {
    const Vertex vertex = tree.vertex(v);
    std::pair<unsigned, unsigned> best{v, 0};

    int parent = vertex.parent();
    if (parent != -1) {
        if (best.second < fu[parent].second + 1) {
            best = std::make_pair(fu[parent].first, fu[parent].second + 1);
        }

        const int sibling = (tree.vertex(parent).left() == (int) v ?
                             tree.vertex(parent).right() : tree.vertex(parent).left());
        if (sibling != -1 && best.second < fd[sibling].second + 2) {
            best = std::make_pair(fd[sibling].first, fd[sibling].second + 2);
        }
    }

    fu[v] = best;

    // pre-order traversal
    for (auto u : {vertex.left(), vertex.right()}) {
        if (u != -1) {
            farUp(tree, (unsigned) u, fu, ancestor2, depth, fd);
        }
    }
}

// The actual solution of the task.
void f() {
    unsigned n, m, s, d;
    int a, b;
    std::cin >> n;

    std::vector<Vertex> vertices;
    for (unsigned i = 0; i < n; ++i) {
        std::cin >> a >> b;
        if (a != -1) {
            --a;
        }
        if (b != -1) {
            --b;
        }
        vertices.emplace_back(i, a, b, -1);
    }
    for (unsigned i = 0; i < n; ++i) {
        for (int u : {vertices[i].left(), vertices[i].right()}) {
            if (u != -1) {
                vertices[u].setParent(i);
            }
        }
    }

    // builds tree
    BinaryTree tree{n, std::move(vertices)};

    // depth
    std::vector<unsigned> depth(n, 0);
    BFS(tree, depth);

    // ancestor2
    std::vector<std::vector<int>> _ancestor2{n};
    ancestor2(tree, _ancestor2);

    // pair<farthest vertex, max distance>
    // far down
    std::vector<std::pair<unsigned, unsigned>> _farDown{n};
    farDown(tree, 0, _farDown);

    // pair<farthest vertex, max distance>
    // far up
    std::vector<std::pair<unsigned, unsigned>> _farUp{n};
    farUp(tree, 0, _farUp, _ancestor2, depth, _farDown);

    // far -- vector of farthest vertices (and max distances) in the whole tree.
    std::vector<std::pair<unsigned, unsigned>> far{std::move(_farDown)};
    for (unsigned i = 0; i < n; ++i) {
        if (far[i].second < _farUp[i].second) {
            far[i] = _farUp[i];
        }
    }

    std::cin >> m;
    for (unsigned i = 0; i < m; ++i) {
        std::cin >> s >> d;
        --s;

        unsigned dMax = far[s].second;
        unsigned f = far[s].first;

        if (dMax < d) {
            std::cout << "-1\n";
        } else if (d == 0) {
            std::cout << s + 1 << '\n';
        } else if (d == dMax) {
            std::cout << f + 1 << '\n';
        } else {
            unsigned l = lca(tree, s, f, _ancestor2, depth);
            unsigned d1 = depth[s] - depth[l];
            if (d <= d1) {
                std::cout << ancestor(tree, s, d, _ancestor2, depth) + 1 << '\n';
            } else {
                std::cout << ancestor(tree, f, dMax - d, _ancestor2, depth) + 1 << '\n';
            }
        }
    }
}

int main() {

    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    f();

    return 0;
}