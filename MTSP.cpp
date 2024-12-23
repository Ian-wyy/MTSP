#include <iostream>
#include <vector>

double **readMap(int n) {
  /* int n = 0;
  std::cin >> n; */
  double **map = new double *[n];
  for (int i = 0; i < n; i++) {
    map[i] = new double[n];
  }
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      std::cin >> map[i][j];
    }
  }
  return map;
}

void testMap(double **map, int n) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      std::cout << map[i][j] << " ";
    }
    std::cout << std::endl;
  }
}

void freeMap(double **map, int n) {
  for (int i = 0; i < n; i++) {
    delete[] map[i];
  }
  delete[] map;
}

int main() {
  int n = 0;
  std::cin >> n;
  double **map = readMap(n);
  testMap(map, n);
  freeMap(map, n);
}