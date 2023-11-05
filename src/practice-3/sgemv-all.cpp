#include <mpi.h>

#include <cmath>
#include <iostream>

const std::string prefix = "[sgemv] ";
enum { m = 10, n = 10 };

void get_chunk(int a, int b, int commsize, int rank, int *lb, int *ub) {
    int n = b - a + 1;
    int q = n / commsize;
    if (n % commsize) q++;
    int r = commsize * q - n;
    /* Compute chunk size for the process */
    int chunk = q;
    if (rank >= commsize - r) chunk = q - 1;
    *lb = a;        /* Determine start item for the process */
    if (rank > 0) { /* Count sum of previous chunks */
        if (rank <= commsize - r) {
            *lb += q * rank;
        } else {
            *lb += q * (commsize - r) + (q - 1) * (rank - (commsize - r));
        }
    }
    *ub = *lb + chunk - 1;
}

/* sgemv: Compute matrix-vector product c[m] = a[m][n] * b[n] */
void sgemv(float *a, float *b, float *c, int m, int n, int commsize, int rank) {
    int lb, ub;
    get_chunk(0, m - 1, commsize, rank, &lb, &ub);
    int nrows = ub - lb + 1;
    for (int i = 0; i < nrows; i++) {
        c[lb + i] = 0.0;
        for (int j = 0; j < n; j++) {
            c[lb + i] += a[i * n + j] * b[j];
        }
    }
}

void printResult(float *result, int m, int commsize, int rank) {
    std::cout << "Result on process " << rank << ": ";
    for (int i = 0; i < m * commsize; i++) {
        std::cout << result[i] << " ";
    }
    std::cout << std::endl;
}

int main(int argc, char **argv) {
    std::cout.setf(std::ios::fixed);

    int commsize, rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &commsize);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double t = MPI_Wtime();

    int lb, ub;
    get_chunk(0, m - 1, commsize, rank, &lb, &ub);
    int nrows = ub - lb + 1;

    float *a = (float *)malloc(sizeof(*a) * nrows * n);
    float *b = (float *)malloc(sizeof(*b) * n);
    float *c = (float *)malloc(sizeof(*c) * m);
    float *result = (float *)malloc(sizeof(*result) * m * commsize); // for all processes

    if (!a || !b || !c || !result) {
        std::cerr << "Malloc of arrays failed\n";
        return 1;
    }

    // Each process initialize their arrays
    for (int i = 0; i < nrows; i++) {
        for (int j = 0; j < n; j++) {
            a[i * n + j] = lb + i + 1;
        }
    }
    for (int j = 0; j < n; j++) {
        b[j] = j + 1;
    }
    sgemv(a, b, c, m, n, commsize, rank);
    // gathering the result on all processes
    MPI_Allgather(c + lb, nrows, MPI_FLOAT, result, nrows, MPI_FLOAT, MPI_COMM_WORLD);

    t = MPI_Wtime() - t;

    //printResult(result, m, commsize, rank);

    // each process outputs the result
    std::cout << prefix << "commsize = " << commsize << ", m = " << m
              << ", n = " << n << ", time = " << t << " sec.\n";
    double gflop = 2.0 * m * n * 1E-9;
    std::cout << prefix << "performance: " << gflop / t << " GFLOPS\n";
    auto used_mem =
            static_cast<uint64_t>(((m * n + m + n) * sizeof(float)) >> 20);
    std::cout << prefix << "used " << used_mem << " MiB of RAM\n";

    free(a);
    free(b);
    free(c);
    free(result);
    MPI_Finalize();
    return 0;
}
