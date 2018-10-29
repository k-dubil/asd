#include <iostream>
#include <vector>

#define MOD 1000000000

using namespace std;


static int f() {
  int n, c1, c2;
  cin >> n;

  vector<int> a(n);
  for (int i = 0; i < n; ++i) {
    cin >> a[i];
  }


  // programowanie dynamiczne:

  // tL(i, j) = z ilu permutacji można otrzymać a[i..j], jeśli ostatnio wstawiono element a[i]
  // tR(i, j) = z ilu permutacji można otrzymać a[i..j], jeśli ostatnio wstawiono element a[j]

  // tL(i, j) = tL(i + 1, j) * [a[i] < a[i + 1]] + tR(i + 1, j) * [a[i] < a[j]]
  // tR(i, j) = tL(i, j - 1) * [a[i] < a[j]] + tR(i, j - 1) * [a[j - 1] < a[j]]

  // wynikiem jest (tL(1, n) + tR(1, n)) % 1000000000
  
  vector<vector<int>> tL(n, vector<int>(n)), tR(n, vector<int>(n));

  for (int i = 0; i < n; ++i) {
    tL[i][i] = 1;
  }

  for (int i = n - 2; i >= 0; --i) {
    for (int j = i + 1; j < n; ++j) {
      c1 = (a[i] < a[i + 1]) ? 1 : 0;
      c2 = (a[i] < a[j]) ? 1 : 0;
      tL[i][j] = (tL[i + 1][j] * c1 + tR[i + 1][j] * c2) % MOD;

      c1 = (a[i] < a[j]) ? 1 : 0;
      c2 = (a[j - 1] < a[j]) ? 1 : 0;
      tR[i][j] = (tL[i][j - 1] * c1 + tR[i][j - 1] * c2) % MOD;
    }
  }

  return (tL[0][n - 1] + tR[0][n - 1]) % MOD;
}

int main() {
  
  cout << f() << endl;

  return 0;
}