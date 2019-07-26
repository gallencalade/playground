#include <iostream>

int max_continue_sum(int* a, int n) {
  if (nullptr == a || 0 == n) {
    return 0;
  }

  int m = a[0];
  int s = a[0];
  for (int i = 1; i < n; ++i) {
    s = std::max(s + a[i], a[i]);
    m = std::max(s, m);
  }

  return m;
}

int main() {
  int arr[9] = { 6, -1, 3, -4, -6, 9, 2, -2, 5 };
  std::cout << max_continue_sum(arr, 9) << std::endl;
}
