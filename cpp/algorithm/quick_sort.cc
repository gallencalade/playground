#include <iostream>

int part(int* arr, int s, int e) {
  int t = arr[s];
  while (s < e) {
    while (s < e && arr[e] >= t) {
      --e;
    }
    arr[s] = arr[e];
    while (s < e && arr[s] <= t) {
      ++s;
    }
    arr[e] = arr[s];
  }
  arr[s] = t;

  return s;
}

void quick_sort(int* arr, int s, int e) {
  if (nullptr == arr || s > e) {
    return ;
  }

  int p = part(arr, s, e);
  quick_sort(arr, s, p - 1);
  quick_sort(arr, p + 1, e);
}

int main() {
  int arr[10] = { 5, 9, 2, 4, 1, 8, 3, 7, 6, 0 };
  quick_sort(arr, 0, 9);
  for (size_t i = 0; i < 10; ++i) {
    std::cout << arr[i] << " ";
  }
  std::cout << std::endl;

  return 0;
}
