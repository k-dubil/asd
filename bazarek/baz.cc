#include <iostream>
#include <vector>
#include <algorithm>


static void f() {
  int n, m;
  long long n_i, k_i, even = -1, odd = -1;
  std::vector<long long> all;

  std::cin >> n;
  for (int i = 0; i < n; ++i) {
    std::cin >> n_i;
    all.push_back(n_i);
  }

  // vector all is now descending
  std::reverse(all.begin(), all.end());

  std::vector<long long> prefix_sums (n);

  // p1(i) = min{A[j] : 0 <= j <= i, A[j] % 2 == 0}
  // n1(i) = max{A[j] : i < j < n, A[j] % 2 == 1}
  // n2(i) = min{A[j] : 0 <= j <= i, A[j] % 2 == 1}
  // p2(i) = max{A[j] : i < j < n, A[j] % 2 == 0}
  std::vector<long long> p1 (n), n1 (n), p2 (n), n2 (n);

  // prefix_sums, p1, n2
  int i = 0;
  std::vector<long long>::iterator it = all.begin();
  prefix_sums[i] = *it;
  if (*it % 2 == 0) {
    even = *it;
  } else {
    odd = *it;
  }
  p1[i] = even;
  n2[i] = odd;
  
  ++it;
  ++i;

  for (; it != all.end(); ++it) {
    prefix_sums[i] = prefix_sums[i - 1] + *it;

    if(*it % 2 == 0) {
      even = *it;
    } else {
      odd = *it;
    }
    p1[i] = even;
    n2[i] = odd;

    ++i;
  }

  // p2, n1
  i = n - 1;
  odd = even = -1;
  std::vector<long long>::reverse_iterator rit = all.rbegin();
  for (; rit != all.rend(); ++rit) {
    p2[i] = even;
    n1[i] = odd;

    if(*rit % 2 == 0) {
      even = *rit;
    } else {
      odd = *rit;
    }

    --i;
  }


  long long res, a1 = -1, a2 = -1;

  std::cin >> m;
  for (int i = 0; i < m; ++i) {
    std::cin >> k_i;

    res = prefix_sums[k_i - 1];
    if (res % 2 == 0) {
      a1 = (p1[k_i - 1] > -1 && n1[k_i - 1] > -1) ? res - p1[k_i - 1] + n1[k_i - 1] : -1;
      a2 = (n2[k_i - 1] > -1 && p2[k_i - 1] > -1) ? res - n2[k_i - 1] + p2[k_i - 1] : -1;

      res = std::max(a1, a2);
      res = (res % 2 == 0) ? -1 : res;
    }

    std::cout << res << '\n';
  }

}

int main() {
  
  f();

  return 0;
}