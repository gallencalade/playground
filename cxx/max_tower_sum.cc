#include <iostream>
#include <vector>

int max_tower_sum(const std::vector<std::vector<int>>& a) {
  if (a.empty()) {
    return 0;
  }

  std::vector<int> vec(a[a.size() - 1]);
  for (int i = a.size() - 2; i >= 0; --i) {
    for (int j = 0; j <= i; ++j) {
      vec[j] = std::max(vec[j], vec[j + 1]) + a[i][j];
    }
  }

  return vec[0];
}

int main() {
  std::vector<std::vector<int>> arr = { { 3 },
                           { 1, 5 },
                           { 8, 4, 3 },
                           { 2, 6, 7, 9 },
                           { 6, 2, 3, 5, 1 }
                         };
  std::cout << max_tower_sum(arr) << std::endl;
}
