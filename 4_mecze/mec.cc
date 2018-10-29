#include <iostream>
#include <vector>
#include <unordered_set>

bool f() {
  int n, m, v_j;
  long long k;
  std::cin >> n >> m;

  std::vector<long long> v (n, 0LL); // we treat 0/1 sequences of length m as bits

  n >>= 1;                           // n := n / 2
  for (int i = 0; i < m; ++i) {      // m rows
    for (int j = 0; j < n; ++j) {    // n numbers in the first half of the row
      std::cin >> v_j;
    }

    for (int j = 0; j < n; ++j) {    // n numbers in the second half of the row
      std::cin >> v_j;

      k = 1LL << i;                  // 2^i
      v[v_j - 1] |= k;               // sets i-th bit to 1
    }
  }

  std::unordered_set<long long> s(v.begin(), v.end());
  return s.size() == (n << 1);       // checks for repetitions in vector v
}

int main() {

  bool res = f();
  std::cout << (res ? "TAK" : "NIE") << '\n';

  return 0;
}