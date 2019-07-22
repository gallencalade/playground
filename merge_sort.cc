#include <iostream>
#include <vector>

void merge_core(int* arr, int s, int m, int e) {
  std::vector<int> vec(e - s + 1);
  int i = s;
  int j = m + 1;
  int k = 0;

  while (i <= m && j <= e) {
    if (arr[i] <= arr[j]) {
      vec[k++] = arr[i++];
    } else {
      vec[k++] = arr[j++];
    }
  }

  while (i <= m) {
    vec[k++] = arr[i++];
  }

  while (j <= e) {
    vec[k++] = arr[j++];
  }

  for (int l = 0; l < vec.size(); ++l) {
    arr[s + l] = vec[l];
  }
}

void merge_sort(int* arr, int s, int e) {
  if (nullptr == arr || s >= e) {
    return ;
  }

  int m = (e + s) / 2;
  merge_sort(arr, s, m);
  merge_sort(arr, m + 1, e);
  merge_core(arr, s, m, e);
}

int main() {
  int arr[10] = { 5, 9, 2, 4, 1, 8, 3, 7, 6, 0 };
  merge_sort(arr, 0, 9);
  for (size_t i = 0; i < 10; ++i) {
    std::cout << arr[i] << " ";
  }
  std::cout << std::endl;

  return 0;
}
