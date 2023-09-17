#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv)
{
	MPI_Init(&argc, &argv);
	int rank, size;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	int *mainbuf = malloc(sizeof(int) * size);
	for(int i = 0; i < size; ++i) {
		mainbuf[i] = -1;
    }
	mainbuf[rank] = rank;
	
	int r = (rank - 1 + size) % size;
	int s = rank;

	double t = MPI_Wtime();
	for (int i = 0; i < size - 1; ++i) {
		printf("I'm #%d, sending to #%d and receiving from #%d\n", rank, (rank + 1) % size, (rank - 1 + size) % size);
		MPI_Sendrecv(&mainbuf[s], 1, MPI_INT, (rank + 1) % size, 0, &mainbuf[r], 1, MPI_INT, (rank - 1 + size) % size, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		s = r;
		r = (r - 1 + size) % size;
	}
	t = MPI_Wtime() - t;

	for(int i = 0; i < size; ++i) {
		if (!(i)) {
			printf("I'm #%d and I contain [%d, ", rank, mainbuf[i]);
        } else {
			if (i == size - 1) {
				printf("%d]\n", mainbuf[i]);
			} else {
				printf("%d, ", mainbuf[i]);
            }
        }
	}
	printf("%f\n", t);
	free(mainbuf);
	MPI_Finalize();
	return 0;
}