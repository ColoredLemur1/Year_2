/* mpi Bradcast 
* K Djemame
* September 2024
*/

#include <stdio.h>
#include "mpi.h"

int main( argc, argv )
int argc;
char **argv;
{
    int rank, size, temp, value;
    MPI_Init( &argc, &argv );

    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size( MPI_COMM_WORLD, &size );

    while (1) {
        if (rank == 0) {
            printf("Input a number (negative to exit): ");
            scanf("%d", &value);
        }

        // Broadcast the value from process 0 to all other processes.
        // This also serves to inform other processes if they should exit.
        MPI_Bcast(&value, 1, MPI_INT, 0, MPI_COMM_WORLD);

        // If the value is negative, all processes break out of the loop.
        if (value < 0) {
            break;
        }

        printf("Process %d received the broadcast value %d\n", rank, value);

        int sum = 0;
        // Each process contributes the 'value' it received.
        // The results are summed and stored in 'sum' on process 0.
        MPI_Reduce(&value, &sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

        // Only the root process (rank 0) prints the final sum for this iteration.
        if (rank == 0) {
            printf("Process 0 calculated the sum to be: %d\n\n", sum);
        }
    }

    printf("Process %d is finalizing.\n", rank);

    MPI_Finalize( );
    return 0;
}

