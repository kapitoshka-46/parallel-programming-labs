#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

const int TAG_PART_OF_FIRST = 12211;
const int TAG_FULL_SECOND = 12345; 
const int TAG_RESULT = 99999;

#define LOG(format, ...) printf("%d: " format "\n", rank, ##__VA_ARGS__)

long long calculate(const int* x, const int* y, int x_length, int y_length) {
    long long s = 0;
    for (int i = 0; i < x_length; i++) {
        for (int j = 0; j < y_length; j++) {
            s += x[i] * y[j];       
        }
    }
    return s;
}


int* create_vector(int legnth);
void delete_vector(int** v);

_Bool is_last_proc(int rank, int size) { return rank == size - 1; }

int part_length(int length, int rank, int size) {
    LOG("is_last_proc(%d, %d) = %d", rank, size, is_last_proc(rank, size));
    return is_last_proc(rank, size) ? length / size + length % size : length / size;
}

void LOG_ARRAY(int rank, const char* name, int* x, int length) {
    printf("%d: %s [", rank, name);
        for (int i = 0; i < length; i++) {
            printf("%d ", x[i]);
        }
        printf("]\n");
}

int main(int argc, char **argv) {
    srand(10000);
    MPI_Init(&argc, &argv);
    
    int rank;
    int size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    LOG("argc=%d", argc);
    if (argc != 2) {
        puts("Specify the vector length");
        puts("Usage:");
        printf("    %s [vector length]\n", argv[0]);
        MPI_Finalize();
        return 0;
    }


    LOG("rank=%d: size=%d", rank, size);
    const int length = atoi(argv[1]);
    const int x_length = part_length(length, rank, size);

    LOG("length=%d", length);
    LOG("x_length=%d", x_length);



    if (rank == 0) {
        // init vectors
        int* x = create_vector(length);
        int* y = create_vector(length);
        if (!x || !y) {
            fputs("Bad malloc. May be not enough memory?", stderr);
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        // ---------- printing vectors ------------ //
        LOG_ARRAY(rank, "1st vector", x, length);
        LOG_ARRAY(rank, "2nd vector", y, length);

        // --------------- send vectors ---------------------//
        for (int i = 1; i < size; i++) {
            if (i == 1) LOG("start sending");

            // send 2nd vector to all processes
            MPI_Send(y, length, MPI_INT, i, TAG_FULL_SECOND, MPI_COMM_WORLD);
            LOG("sended y");
            
            // send part of 1st vector to all processes
            LOG("sending part of x...");
            int length_send = part_length(length, i, size);
            LOG_ARRAY(rank, "\tpart of x", x + i*x_length, length_send);
            MPI_Send(x + i*x_length, length_send, MPI_INT, i, TAG_PART_OF_FIRST, MPI_COMM_WORLD);
            LOG("sending complete!");
        }       

        // -------------- calculate part ----------------//
        LOG("call calculating");
        long long sum = calculate(x, y, x_length, length);
        LOG("calculated sum=%lld", sum);

        // ---------------- merge results -------------------//
        for (int i = 1; i < size; i++) {
            if (i == 1) LOG("waiting results");
            int recieve;
            MPI_Recv(&recieve, 1, MPI_LONG_LONG, i, TAG_RESULT, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            sum += recieve;
            if (i == size - 1) LOG("got results");
        }
        printf("%lld\n", sum); 
        delete_vector(&x);
        delete_vector(&y);
    }
    else {  // can't enter this block if size == 1 becuse (rank < size) => (rank == 0)
        
        // --------------- recieve vectors -------------------// 
        // ---------------- 2nd vector -------------------
        LOG("recieving 2nd vector");
        int* y = malloc(sizeof(int) * length);    
        if (!y) MPI_Abort(MPI_COMM_WORLD, 2);
        MPI_Recv(y, length, MPI_INT, 0, TAG_FULL_SECOND, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        LOG_ARRAY(rank, "\trecieved 2nd vector", y, length);

        // ---------------- first vector -----------------
        LOG("recieving part of 1st vector");
        int x_length = part_length(length, rank, size);
        int* x = malloc(sizeof(int) * x_length);
        MPI_Recv(x, x_length, MPI_INT, 0, TAG_PART_OF_FIRST, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        LOG_ARRAY(rank, "\trecieved part of 1st", x, x_length);
        LOG("all data recieved");

        // ----------------- calculate -------------------- //
        LOG("call calculating");
        long long sum = calculate(x, y, x_length, length);
        LOG("calculated sum=%lld", sum);
        
        // --------------- merge to rank0 ----------------- //
        LOG("sending results to rank0");
        MPI_Send(&sum, 1, MPI_LONG_LONG, 0, TAG_RESULT, MPI_COMM_WORLD);       
    }

    LOG("MPI_Finalize()");
    MPI_Finalize();
    return 0;
}



// ============== Vector functions ===============

int* create_vector(int length) {
    int* vector = malloc(sizeof(int) * length);
    if (!vector) return NULL;
    for (int i = 0; i < length; i++) { vector[i] = rand() % 10; }
    return vector;
}

void delete_vector(int** v) {
    if (*v) free(*v);
    *v = NULL;
}