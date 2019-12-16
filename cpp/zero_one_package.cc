#include <iostream>
#include <vector>

int zero_one_package(const std::vector<int>& w, const std::vector<int>& p, int v) {
  std::vector<int> dp(v + 1, 0);
  for (int i = 0; i < (int)w.size(); ++i) {
    for (int j = v; j > w[i]; --j) {
      dp[j] = std::max(dp[j], dp[j - w[i]] + p[i]);
    }
  }

  return dp[v];
}

int main() {
  std::vector<int> w({ 5, 6, 5, 1, 19, 7 });
  std::vector<int> p({ 2, 3, 1, 4, 6, 5 });
  std::cout << zero_one_package(w, p, 10) << std::endl;

  return 0;
}
