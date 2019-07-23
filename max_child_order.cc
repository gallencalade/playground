#include <iostream>
#include <vector>
#include <algorithm>

int max_child_order(int* a, int n) {
  if (nullptr == a || 0 == n) {
    return 0;
  }

  std::vector<int> vec(n, 1);
  for(int i = 1;i < n; ++i){
    for(int j = 0; j < i; ++j){
      if(a[i] > a[j] && vec[j] + 1 > vec[i]){
        vec[i] = vec[j]+1;
      }
    }
  }

  auto pos = std::max_element(vec.cbegin(), vec.cend());

  return *pos;
}

int main() {
  int a[9] = { 3, 1, 4, 1, 5, 9, 2, 6, 5 };
  std::cout << max_child_order(a, 9) << std::endl;
  return 0;
}
