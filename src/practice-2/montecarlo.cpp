#include <mpi.h>

#include <cmath>
#include <iostream>

const double PI = 3.14159295;
const int n = 1000;

double get_rand() { return static_cast<double>(rand()) / RAND_MAX; }

double funcX(double y) { return y * y * y; }

int main(int argc, char **argv) {
  int rank, commsize;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &commsize);

  srand(rank);
  int in = 0;
  double s = 0;
  for (int i = rank; i < n; i += commsize) {
    double x = get_rand();         /* x in [0, 1] */
    double y = get_rand() * 7 - 2; /* y in [2, 5] */
    if (x <= funcX(y)) {
      in++;
      s += funcX(y);
    }
  }

  int gin = 0;
  MPI_Reduce(&in, &gin, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
  double gsum = 0.0;
  MPI_Reduce(&s, &gsum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
  if (rank == 0) {
    double v = PI * gin / n;
    double res = v * gsum / gin;
    std::cout << "[montecarlo] commsize = " << commsize << ", S = " << res
              << ", n = " << n << '\n';
  }

  MPI_Finalize();
  return 0;
}