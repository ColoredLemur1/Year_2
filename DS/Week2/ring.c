/* Sending data in a ring 
* K Djemame
* September 2024
*/

#include <stdio.h>
#include "mpi.h"

int main( argc, argv )
int argc;
char **argv;
{
    int rank, value, size;
    MPI_Status status;

    MPI_Init( &argc, &argv );

    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size( MPI_COMM_WORLD, &size );
    

   char         processorname[100];
   int          namelen;
   MPI_Get_processor_name( processorname, &namelen );
    if (rank == 0) {
	    printf("Input a value : ");
        scanf( "%d", &value );
	    MPI_Send( &value, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD );
		/* Now, receive from the last process */
		MPI_Recv( &value, 1, MPI_INT, size - 1, 0, MPI_COMM_WORLD, &status );
	}
	else {
	    MPI_Recv( &value, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, 
		      &status );
        value++;
	    if (rank < size - 1)
        {
		MPI_Send( &value, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD );
        }
		else /* Last process, send back to rank 0 */
		{
			MPI_Send( &value, 1, MPI_INT, 0, 0, MPI_COMM_WORLD );
		}
	}
	printf( "Process %d on %s got %d\n", rank, processorname, value );

    MPI_Finalize( );
    return 0;
}

