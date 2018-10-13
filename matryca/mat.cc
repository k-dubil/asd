#include <iostream>
#include <limits>


static int f() {
  std::string s;
  char previousChar = '\0';
  int i = 0, previousIndex = -1;
  int minDist = std::numeric_limits<int>::max();

  std::cin >> s;

  for (auto c : s) {
    if (c != '*' && c != previousChar) {
      previousChar = c;
      minDist = (previousIndex > -1) ? std::min(minDist, i - previousIndex) : minDist;
      previousIndex = i;
    } else if (c == previousChar) {
      previousIndex = i;
    }

    ++i;
  }

  if (previousIndex == -1 || i <= minDist) {
    return 1;
  }
  return i - minDist + 1;
}

int main() {
  
  std::cout << f() << std::endl;

  return 0;
}