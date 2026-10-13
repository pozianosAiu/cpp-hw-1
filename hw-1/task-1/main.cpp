#include <iostream>

struct Pair {
  int n1;
  int n2;
};

double Average(int* arr, int size) {
  long long sum{};
  for (int i = 0; i < size; ++i) {
    sum += arr[i];
  }
  return 1.0 * sum / size;
}

Pair MinMax(int* arr, int size) {
  int max{arr[0]};
  int min{arr[0]};
  for (int i = 1; i < size; ++i) {
    if (arr[i] > max) {
      max = arr[i];
    }
    if (arr[i] < min) {
      min = arr[i];
    }
  }
  return {min, max};
}

int BelowZeroCounter(int* arr, int size) {
  int cnt{0};
  for (int i = 0; i < size; ++i) {
    if (arr[i] < 0) {
      ++cnt;
    }
  }
  return cnt;
}

int main() {
  int size;
  std::cin >> size;
  int* temperatures = new int[size];
  for (int i = 0; i < size; ++i) {
    std::cin >> temperatures[i];
  }

  std::cout << std::fixed;
  std::cout.precision(1);
  std::cout << Average(temperatures, size) << '\n';
  Pair minmax = MinMax(temperatures, size);
  std::cout << minmax.n1 << ' ' << minmax.n2 << '\n';
  std::cout << BelowZeroCounter(temperatures, size) << '\n';
  delete[] temperatures;
  return 0;
}