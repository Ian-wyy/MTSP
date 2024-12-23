#include <iostream>
#include <vector>

#define INF 100000

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

double **TSP_dp(double **map, int n) {
  int m = 1 << (n - 1);
  double **dp = new double *[n];
  for (int i = 0; i < n; i++) {
    dp[i] = new double[m];
  }
  for (int i = 0; i < n; i++) {
    dp[i][0] = map[i][0];
  }

  for (int j = 1; j < m; j++) {
    for (int i = 0; i < n; i++) {
      dp[i][j] = INF;
      if (((j >> (i - 1)) & 1) == 1) {
        continue;
      }
      for (int k = 1; k < n; k++) {
        if (((j >> (k - 1)) & 1) == 0) {
          continue;
        }
        if (dp[i][j] > map[i][k] + dp[k][j ^ (1 << (k - 1))]) {
          dp[i][j] = map[i][k] + dp[k][j ^ (1 << (k - 1))];
        }
      }
    }
  }

  return dp;
}

void freeDP(double **dp, int n) {
  for (int i = 0; i < n; i++) {
    delete[] dp[i];
  }
  delete[] dp;
}

int main() {
  int n = 0;
  std::cin >> n;
  int m = 1 << (n - 1);
  double **map = readMap(n);
  double **dp = TSP_dp(map, n);
  std::cout << dp[0][m - 1] << std::endl;
  freeMap(map, n);
  freeDP(dp, n);
}