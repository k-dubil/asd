#include <iostream>
#include <cmath>
#include <vector>

// Three possible colors of the nodes.
enum class Color {
    BLACK, WHITE, MIXED
};

class Node {

    Color color; // Color of the node.
    int value;   // Number of white descendants.

public:

    Node(Color c, int v) : color(c), value(v) {}

    Node() = delete;

    Color getColor() const { return color; }

    int getValue() const { return value; }

    void setColor(Color c) { color = c; }

    void setValue(int v) { value = v; }
};

class IntervalTree {

public:
    IntervalTree() = delete;

    explicit IntervalTree(int n);

    // Initiates the tree (all nodes to Color::BLACK).
    void init();

    // Updates the tree – colors the [l, r] interval in color c; v = [vl, vr] is parameter of the current call.
    void update(int l, int r, Color c, int v, int vl, int vr);

    // Returns the number of white descendants of the node v = [vl, vr].
    int query(int v, int vl, int vr);

private:
    int N; // Number of elements.
    int M; // Closest power of 2 not less than N.
    std::vector<Node> vec; // vec.size() == 2 * M - 1 after init().

    // Unpacks the node – changes white/black node to mixed, its children into its previous color, sets values.
    void unpack(int v, int vl, int vr);

    static inline int left(int v) { return 2 * v; }

    static inline int right(int v) { return 2 * v + 1; }

    inline Color color(int v) const { return vec[v - 1].getColor(); }

    inline int value(int v) const { return vec[v - 1].getValue(); }

};

IntervalTree::IntervalTree(int n) : N(n), M((int) pow(2.0, ceil(log2(double(N))))) {
    init();
}

void IntervalTree::init() {
    for (int i = 0; i < 2 * M - 1; ++i) {
        vec.emplace_back(Node{Color::BLACK, 0});
    }
}

void IntervalTree::unpack(int v, int vl, int vr) {
    int vm = (vl + vr) / 2;
    vec[left(v) - 1].setColor(color(v));
    vec[right(v) - 1].setColor(color(v));
    vec[v - 1].setColor(Color::MIXED);
    vec[left(v) - 1].setValue(color(left(v)) == Color::WHITE ? vm - vl + 1 : 0);
    vec[right(v) - 1].setValue(color(right(v)) == Color::WHITE ? vr - vm : 0);
}

void IntervalTree::update(int l, int r, Color c, int v, int vl, int vr) {
    if (vr < l || r < vl) {
        return;
    }
    if (l <= vl && vr <= r) {
        vec[v - 1].setColor(c);
        vec[v - 1].setValue(c == Color::WHITE ? vr - vl + 1 : 0);
    } else {
        if (color(v) != Color::MIXED) {
            unpack(v, vl, vr);
        }

        int vm = (vl + vr) / 2;
        update(l, r, c, left(v), vl, vm);
        update(l, r, c, right(v), vm + 1, vr);
        vec[v - 1].setValue(value(left(v)) + value(right(v)));
    }
}

int IntervalTree::query(int v, int vl, int vr) {
    switch (color(v)) {
        case Color::WHITE :
            return vr - vl + 1;
        case Color::BLACK :
            return 0;
        default:
            return value(left(v)) + value(right(v));
    }

}

// The actual solution of the task.
void f() {
    int n, m, a, b;
    char c;
    std::cin >> n >> m;

    IntervalTree tree{n};

    for (int i = 0; i < m; ++i) {
        std::cin >> a >> b >> c;
        Color color = (c == 'B' ? Color::WHITE : Color::BLACK);

        tree.update(a, b, color, 1, 1, n);

        std::cout << tree.query(1, 1, n) << std::endl;
    }
}

int main() {

    f();

    return 0;
}