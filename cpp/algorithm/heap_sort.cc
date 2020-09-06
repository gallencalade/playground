#include <iostream>

void heapify(int* arr, int s, int e) {
  // save the starting node, the one will be down.
  int t = arr[s];
  // compare the two nodes, in fact that the loop is only executed twice.
  for (int i = 2 * s + 1; i <= e; i *= 2) {
    // find the max value node index.
    if (i < e && arr[i] < arr[i + 1]) {
      ++i;
    }
    // compare the max child and parent value, and swap if necessary.
    if (t < arr[i]) {
      arr[s] = arr[i];
      s = i;
    }
  }

  // restore the first node.
  arr[s] = t;
}

void heap_sort(int* arr, int n) {
  if (nullptr == arr || 0 == n) {
    return ;
  }

  int i;
  // step 1: adjust the local max heap.
  // adjust the comparation from the last node which has a child to the root node.
  for (i = n / 2 - 1; i >= 0; --i) {
    heapify(arr, i, n - 1);
  }

  // step 2: adjust the global max heap.
  for (i = n - 1; i > 0; --i) {
    // select the max value in the root, and heapify the others
    std::swap(arr[0], arr[i]);
    heapify(arr, 0, i - 1);
  }
}

int main() {
  int arr[10] = { 5, 9, 2, 4, 1, 8, 3, 7, 6, 0 };
  heap_sort(arr, 10);
  for (size_t i = 0; i < 10; ++i) {
    std::cout << arr[i] << " ";
  }
  std::cout << std::endl;

  return 0;
}
