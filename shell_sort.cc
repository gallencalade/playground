#include <iostream>

void shell_core(int* arr, int n, int d) {
  for (int i = d; i < n; ++i) {
    int j = i - d;
    int t = arr[i];
    while (j >= 0 && arr[j] > t) {
      arr[j + d] = arr[j];
      j -= d;
    }
    if (j != i - d) {
      arr[j + d] = t;
    }
  }
}

void shell_sort(int* arr, int n) {
  if (nullptr == arr || 0 == n) {
    return ;
  }

  for (int i = n / 2; i >= 1; i /= 2) {
    shell_core(arr, n, i);
  }
}

int main() {
  int arr[10] = { 5, 9, 2, 4, 1, 8, 3, 7, 6, 0 };
  shell_sort(arr, 10);
  for (size_t i = 0; i < 10; ++i) {
    std::cout << arr[i] << " ";
  }
  std::cout << std::endl;

  return 0;
}
