#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <omp.h>
#define ll long long

int main(int argc, char *argv[])
{
    int process, size, range;
    int *arr;
    ll sum = 0, total = 0;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &process);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (process == 0) 
    {
        int N;
        printf("Enter array size: ");
        fflush(stdout);
        scanf("%d", &N);
        arr = (int *)malloc(N * sizeof(int));
        printf("Array: ");
        fflush(stdout);
        for (int i = 0; i < N; ++i)
        {
            scanf("%d", &arr[i]);
        }
        range = N / size;
        int currentpos = range;
        for (int i = 1; i < size - 1; i++)
        {
            MPI_Send(&range, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&arr[currentpos], range, MPI_INT, i, 0, MPI_COMM_WORLD);
            currentpos += range;
        }
        int workLeft = N - currentpos;
        MPI_Send(&workLeft, 1, MPI_INT, size - 1, 0, MPI_COMM_WORLD);
        MPI_Send(&arr[currentpos], workLeft, MPI_INT, size - 1, 0, MPI_COMM_WORLD);
    }
    else
    {
        MPI_Recv(&range, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        arr = (int *)malloc(range * sizeof(int));
        MPI_Recv(arr, range, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    #pragma omp parallel for reduction(+ : sum)
    for (int i = 0; i < range; i++)
    {
        sum += arr[i];
    }
    printf("Process %d calculated sum: %lld\n", process, sum);
    MPI_Reduce(&sum, &total, 1, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
    if (process == 0)
    {
        printf("Total sum: %lld\n", total);
        fflush(stdout);
    }
    free(arr);
    MPI_Finalize();

    return 0;
}