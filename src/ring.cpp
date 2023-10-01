#include <mpi.h>

#include <iostream>

int main(/*int argc, char **argv*/) {
    // int rank, size, next, prev, bufsize = 1;
    // MPI_Init(&argc, &argv);
    // MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    // MPI_Comm_size(MPI_COMM_WORLD, &size);
    // char *buf = (char*)malloc(sizeof(char) * size * bufsize);
    // next = (rank + 1) % size;
    // prev = (rank + size - 1) % size;
    // buf[0] = 'a' + rank;
    // if (rank == 0) {
    //     printf("Process 0 sending '%c' to %d, tag %d (%d processes in ring)\n", message, next, tag,
    //            size);
    //     MPI_Send(&message, 1, MPI_INT, next, tag, MPI_COMM_WORLD);
    //     printf("Process 0 sent to %d\n", next);
    // }

    // /* Pass the message around the ring.  The exit mechanism works as
    //    follows: the message (a positive integer) is passed around the
    //    ring.  Each time it passes rank 0, it is decremented.  When
    //    each processes receives a message containing a 0 value, it
    //    passes the message on to the next process and then quits.  By
    //    passing the 0 message first, every process gets the 0 message
    //    and can quit normally. */

    // while (1) {
    //     MPI_Recv(&message, 1, MPI_INT, prev, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    //     if (0 == rank) {
    //         --message;
    //         printf("Process 0 decremented value: %d\n", message);
    //     }

    //     MPI_Send(&message, 1, MPI_INT, next, tag, MPI_COMM_WORLD);
    //     if (0 == message) {
    //         printf("Process %d exiting\n", rank);
    //         break;
    //     }
    // }

    // /* The last process does one extra send to process 0, which needs
    //    to be received before the program can exit */

    // if (0 == rank) {
    //     MPI_Recv(&message, 1, MPI_INT, prev, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    // }

    // MPI_Finalize();
    return 0;
}