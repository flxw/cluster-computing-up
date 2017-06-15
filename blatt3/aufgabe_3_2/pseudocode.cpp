
// Pseudocode for MPI_Barrier implementation.
int MPI_Barrier(MPI_Comm comm)
{
    int comm_size;
    MPI_Comm_size(comm, &comm_size);
    if (rank > 0)
    {
        err = MPI_Send(NULL, 0, MPI_BYTE, 0, MPI_ANY_TAG, comm);
        if (err != MPI_SUCCESS)
        {
            return err;
        }

        err = MPI_Recv(NULL, 0, MPI_BYTE, 0, MPI_TAG_ANY, comm);
        if (err != MPI_SUCCESS)
        {
            return err;
        }
    }
    else
    {
        for (int i = 0; i < comm_size; i++)
        {
            err = MPI_Recv(NULL, 0, MPI_BYTE, MPI_SOURCE_ANY, MPI_TAG_ANY, comm);
            if (err != MPI_SUCCESS)
            {
                return err;
            }
        }

        for (int i = 0; i < comm_size; i++)
        {
            err = MPI_Send(NULL, 0, MPI_BYTE, i, MPI_TAG_ANY, comm);
            if (err != MPI_SUCCESS)
            {
                return err;
            }
        }
    }
}
