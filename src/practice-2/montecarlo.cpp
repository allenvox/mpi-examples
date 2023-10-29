#include <mpi.h>

#include <cmath>
#include <iostream>

const int n = 100000000;

double get_rand() { return static_cast<double>(rand()) / RAND_MAX; }

double funcX(double y) { return y * y * y; }

int main(int argc, char **argv) {
  std::cout.setf(std::ios::fixed);

  int rank, commsize;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &commsize);
  srand(rank);
  int in = 0;
  double s = 0;

  double t = MPI_Wtime();
  for (int i = rank; i < n; i += commsize) {
    double x = get_rand();         /* x in [0, 1] */
    double y = get_rand() * 7 - 2; /* y in [2, 5] */
    if (x <= funcX(y)) {
      in++;
      s += funcX(y);
    }
  }

  int global_in = 0;
  MPI_Reduce(&in, &global_in, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
  double global_sum = 0.0;
  MPI_Reduce(&s, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
  t = MPI_Wtime() - t;

  if (rank == 0) {
    double v = static_cast<double>(global_in) / n;
    double res = v * global_sum / global_in;
    std::cout << "[montecarlo] S = " << res << ", t = " << t
              << ", commsize = " << commsize << ", n = " << n << '\n';
  }

  MPI_Finalize();
  return 0;
}