#include <mpi.h>

#include <cmath>
#include <iostream>

const double eps = 1E-6;
const int n0 = 100;
const double a = -1;
const double b = 1;

double func(double x) { return (sin(x + 2)) / (0.4 + cos(x)); }

int main(int argc, char **argv) {
  std::cout.setf(std::ios::fixed);

  int commsize, rank;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &commsize);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int n = n0, k;
  double sq[2], delta = 1;

  double t = MPI_Wtime();
  for (k = 0; delta > eps; n *= 2, k ^= 1) {
    int points_per_proc = n / commsize;
    int lb = rank * points_per_proc;
    int ub = (rank == commsize - 1) ? (n - 1) : (lb + points_per_proc - 1);
    double h = (b - a) / n;
    double s = 0.0;
    for (int i = lb; i <= ub; i++) {
      s += func(a + h * (i + 0.5));
    }
    MPI_Allreduce(&s, &sq[k], 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
    sq[k] *= h;
    if (n > n0) {
      delta = fabs(sq[k] - sq[k ^ 1]) / 3.0;
    }
  }
  t = MPI_Wtime() - t;

  if (rank == 0) {
    std::cout << "[midpoint] S = " << sq[k] << ", t = " << t << ", eps = "
              << eps << ", n = " << n / 2 << '\n';
  }
  MPI_Finalize();
  return 0;
}